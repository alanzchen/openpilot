#!/usr/bin/env python3
"""Package verified device outputs and apply them without executing build code.

The publisher runs this trusted script on a clean checkout of the exact source
commit. Archive metadata never chooses its own writable-path policy: that comes
from the immutable, previously validated device-build inventory below.
"""

import argparse
from contextlib import contextmanager
import hashlib
import io
import json
import math
import os
from pathlib import Path, PurePosixPath
import re
import secrets
import stat
import subprocess
import tarfile


BASELINE_SHA = "967cc94dc9634ae5bb170fd3ea415318e234713a"
BASELINE_INVENTORY = "build_stamps/mac-larch64.json"
STAMP_PATH = "build_stamps/linux-device-build.json"
CONTROL_NAME = "manifest.json"
MAX_JSON_BYTES = 8 * 1024 * 1024
MAX_FILE_BYTES = 512 * 1024 * 1024
MAX_ARCHIVE_BYTES = 2 * 1024 * 1024 * 1024
SHA_RE = re.compile(r"[0-9a-f]{40}\Z")
HASH_RE = re.compile(r"[0-9a-f]{64}\Z")
MODES = {"100644": 0o644, "100755": 0o755}
PREBUILT = {"sha256": hashlib.sha256(b"").hexdigest(), "mode": "100644"}


def fail(message):
  raise ValueError(message)


def git(root, *args):
  result = subprocess.run(["git", "-C", str(root), *args], capture_output=True, check=False)
  if result.returncode:
    fail(f"Git verification failed: {' '.join(args)}: {result.stderr.decode(errors='replace').strip()}")
  return result.stdout


def canonical_json(value):
  return (json.dumps(value, sort_keys=True, indent=2, allow_nan=False) + "\n").encode()


def _unique_object(pairs):
  result = {}
  for key, value in pairs:
    if key in result:
      fail(f"Duplicate JSON key: {key}")
    result[key] = value
  return result


def _finite_float(value):
  number = float(value)
  if not math.isfinite(number):
    fail("Manifest numbers must be finite")
  return number


def read_json(data):
  if len(data) > MAX_JSON_BYTES:
    fail("Manifest is too large")
  try:
    value = json.loads(data, object_pairs_hook=_unique_object, parse_float=_finite_float,
                       parse_constant=lambda value: fail(f"Invalid JSON constant: {value}"))
  except (UnicodeDecodeError, json.JSONDecodeError) as error:
    fail(f"Invalid manifest JSON: {error}")
  if not isinstance(value, dict):
    fail("Manifest must be an object")
  return value


def valid_path(value):
  if not isinstance(value, str) or not value or "\\" in value or "\x00" in value:
    fail("Invalid artifact path")
  parts = value.split("/")
  if any(part in ("", ".", "..", ".git") for part in parts) or PurePosixPath(value).is_absolute():
    fail(f"Unsafe artifact path: {value}")
  return value


def checkout(root, source_sha, *, clean):
  if not isinstance(source_sha, str) or SHA_RE.fullmatch(source_sha) is None:
    fail("Expected a full lowercase source commit SHA")
  root = Path(root).absolute()
  if root.is_symlink() or not root.is_dir():
    fail("Checkout root must be a real directory")
  top = Path(git(root, "rev-parse", "--show-toplevel").decode().strip())
  if top.resolve() != root.resolve():
    fail("Root must be the checkout's top-level directory")
  if git(root, "rev-parse", "HEAD").decode().strip() != source_sha:
    fail("Checkout HEAD does not match the expected source commit")
  if clean and git(root, "status", "--porcelain=v1", "-z", "--untracked-files=all"):
    fail("Publisher checkout must be clean")
  return root


def trusted_artifacts(root, source_sha):
  baseline = read_json(git(root, "show", f"{BASELINE_SHA}:{BASELINE_INVENTORY}"))
  inventory = baseline.get("package_inventory")
  if not isinstance(inventory, dict) or not inventory:
    fail("Trusted baseline inventory is unavailable")
  allowed = {valid_path(path) for path in inventory}
  if "prebuilt" not in allowed or STAMP_PATH in allowed:
    fail("Trusted baseline inventory has unexpected control paths")
  tracked = {}
  for entry in git(root, "ls-tree", "-r", "-z", "--full-tree", source_sha).split(b"\0"):
    if not entry:
      continue
    metadata, path = entry.split(b"\t", 1)
    mode, kind, _sha = metadata.decode().split()
    tracked[path.decode()] = (mode, kind)
  if any(tracked.get(path, (None, None))[0] not in MODES or tracked[path][1] != "blob" for path in allowed):
    fail("Every baseline artifact must be a tracked regular file in the source commit")
  return allowed


def validate_manifest(manifest, source_sha, allowed, *, include_prebuilt):
  if type(manifest.get("schema")) is not int or manifest["schema"] != 1 or manifest.get("verdict") != "PASS":
    fail("A passing schema-1 gate manifest is required")
  if manifest.get("source_sha") != source_sha:
    fail("Manifest source SHA does not match the expected commit")
  artifacts = manifest.get("artifacts")
  expected = allowed if include_prebuilt else allowed - {"prebuilt"}
  if not isinstance(artifacts, dict) or artifacts.keys() != expected:
    fail("Artifact inventory must exactly match the trusted baseline")
  for path, metadata in artifacts.items():
    valid_path(path)
    if not isinstance(metadata, dict) or metadata.keys() != {"sha256", "mode"}:
      fail(f"Invalid artifact metadata: {path}")
    if (not isinstance(metadata["sha256"], str) or HASH_RE.fullmatch(metadata["sha256"]) is None or
        not isinstance(metadata["mode"], str) or metadata["mode"] not in MODES):
      fail(f"Invalid artifact hash or mode: {path}")
  if include_prebuilt and artifacts["prebuilt"] != PREBUILT:
    fail("The prebuilt marker must be empty and non-executable")
  return artifacts


@contextmanager
def parent_directory(root, relative):
  """Resolve each directory with openat/O_NOFOLLOW, including during writes."""
  parts = valid_path(relative).split("/")
  flags = os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW
  current = os.open(root, flags)
  try:
    for part in parts[:-1]:
      child = os.open(part, flags, dir_fd=current)
      os.close(current)
      current = child
    yield current, parts[-1]
  finally:
    os.close(current)


def regular_file(root, path):
  with parent_directory(root, path) as (parent, name):
    fd = os.open(name, os.O_RDONLY | os.O_NOFOLLOW, dir_fd=parent)
    try:
      info = os.fstat(fd)
      if not stat.S_ISREG(info.st_mode) or info.st_nlink != 1 or info.st_size > MAX_FILE_BYTES:
        fail(f"Artifact must be a bounded regular file, without hard links: {path}")
      with os.fdopen(fd, "rb", closefd=False) as stream:
        data = stream.read(MAX_FILE_BYTES + 1)
      if len(data) != info.st_size:
        fail(f"Artifact changed while reading: {path}")
      mode = "100755" if info.st_mode & 0o111 else "100644"
      return data, {"sha256": hashlib.sha256(data).hexdigest(), "mode": mode}
    finally:
      os.close(fd)


def artifact_only_changes(root, allowed):
  changed = git(root, "diff", "--no-renames", "--name-only", "-z", "HEAD", "--").split(b"\0")
  if any(path.decode() not in allowed for path in changed if path):
    fail("Build checkout contains changes outside the artifact inventory")


def outside_checkout(root, path):
  path = Path(path).absolute()
  if path.resolve().is_relative_to(root.resolve()):
    fail("Archive must be outside the source checkout")
  return path


def package(root, manifest_path, output, source_sha):
  root = checkout(root, source_sha, clean=False)
  allowed = trusted_artifacts(root, source_sha)
  artifact_only_changes(root, allowed)
  manifest = read_json(Path(manifest_path).read_bytes())
  artifacts = validate_manifest(manifest, source_sha, allowed, include_prebuilt=False)
  payload = {}
  for path, expected in artifacts.items():
    data, actual = regular_file(root, path)
    if actual != expected:
      fail(f"Artifact does not match gate manifest: {path}")
    payload[path] = data
  # An explicit control marker is created only after every build output passes.
  manifest["artifacts"] = {**artifacts, "prebuilt": dict(PREBUILT)}
  payload["prebuilt"] = b""
  output = outside_checkout(root, output)
  temporary = output.with_name(f".{output.name}.{secrets.token_hex(8)}.tmp")
  try:
    with open(temporary, "xb") as destination, tarfile.open(fileobj=destination, mode="w", format=tarfile.USTAR_FORMAT) as archive:
      members = {CONTROL_NAME: canonical_json(manifest), **{"artifacts/" + path: data for path, data in payload.items()}}
      for name in sorted(members):
        content = members[name]
        info = tarfile.TarInfo(name)
        info.size = len(content)
        info.mode = MODES[manifest["artifacts"][name[10:]]["mode"]] if name.startswith("artifacts/") else 0o644
        info.mtime = 0
        archive.addfile(info, io.BytesIO(content))
    if temporary.stat().st_size > MAX_ARCHIVE_BYTES:
      fail("Artifact archive is too large")
    os.replace(temporary, output)
  finally:
    temporary.unlink(missing_ok=True)
  return output


def archive_payload(archive_path):
  if archive_path.is_symlink() or not archive_path.is_file() or archive_path.stat().st_size > MAX_ARCHIVE_BYTES:
    fail("Archive must be a bounded regular file")
  payload = {}
  total = 0
  with tarfile.open(archive_path, "r:") as archive:
    for member in archive:
      name = valid_path(member.name)
      if not member.isreg() or member.pax_headers or member.linkname or name in payload:
        fail(f"Archive contains duplicate or non-regular members: {name}")
      if name != CONTROL_NAME and not name.startswith("artifacts/"):
        fail(f"Unexpected archive member: {name}")
      limit = MAX_JSON_BYTES if name == CONTROL_NAME else MAX_FILE_BYTES
      total += member.size
      if member.size < 0 or member.size > limit or total > MAX_ARCHIVE_BYTES:
        fail("Archive content exceeds size limits")
      stream = archive.extractfile(member)
      data = stream.read(limit + 1)
      if len(data) != member.size or member.mode not in MODES.values():
        fail(f"Invalid member size or permissions: {name}")
      payload[name] = (data, member.mode)
  if CONTROL_NAME not in payload:
    fail("Archive has no manifest")
  return payload


def write_regular(root, path, data, mode):
  with parent_directory(root, path) as (parent, name):
    try:
      info = os.stat(name, dir_fd=parent, follow_symlinks=False)
      if not stat.S_ISREG(info.st_mode) or info.st_nlink != 1:
        fail(f"Refusing to replace a non-regular path: {path}")
    except FileNotFoundError:
      if path != STAMP_PATH:
        fail(f"Tracked artifact is missing: {path}")
    temporary = f".linux-device-{secrets.token_hex(8)}.tmp"
    fd = os.open(temporary, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, 0o600, dir_fd=parent)
    try:
      with os.fdopen(fd, "wb", closefd=False) as stream:
        stream.write(data)
        stream.flush()
        os.fsync(fd)
      os.fchmod(fd, mode)
      os.replace(temporary, name, src_dir_fd=parent, dst_dir_fd=parent)
    finally:
      os.close(fd)
      try:
        os.unlink(temporary, dir_fd=parent)
      except FileNotFoundError:
        pass


def apply(root, archive, expected_source):
  root = checkout(root, expected_source, clean=True)
  allowed = trusted_artifacts(root, expected_source)
  archive = outside_checkout(root, archive)
  payload = archive_payload(archive)
  manifest = read_json(payload[CONTROL_NAME][0])
  artifacts = validate_manifest(manifest, expected_source, allowed, include_prebuilt=True)
  expected_members = {CONTROL_NAME, *("artifacts/" + path for path in artifacts)}
  if payload.keys() != expected_members:
    fail("Archive contents do not exactly match its manifest")
  for path, metadata in artifacts.items():
    data, mode = payload["artifacts/" + path]
    if hashlib.sha256(data).hexdigest() != metadata["sha256"] or mode != MODES[metadata["mode"]]:
      fail(f"Archive artifact does not match its manifest: {path}")
    # Check every checkout path before the first mutation, including parents.
    regular_file(root, path)
  with parent_directory(root, STAMP_PATH) as (parent, name):
    try:
      info = os.stat(name, dir_fd=parent, follow_symlinks=False)
      if not stat.S_ISREG(info.st_mode) or info.st_nlink != 1:
        fail("Provenance stamp path is not a regular file")
    except FileNotFoundError:
      pass
  stamp = {**manifest, "schema": "sp-linux-device-build/1",
           "manifest_sha256": hashlib.sha256(payload[CONTROL_NAME][0]).hexdigest(),
           "archive_sha256": hashlib.sha256(archive.read_bytes()).hexdigest()}
  stamp_data = canonical_json(stamp)
  for path, metadata in artifacts.items():
    write_regular(root, path, payload["artifacts/" + path][0], MODES[metadata["mode"]])
  write_regular(root, STAMP_PATH, stamp_data, 0o644)
  artifact_only_changes(root, allowed | {STAMP_PATH})
  return stamp


def verify_index(root, expected_source):
  """Check committed bytes after git-add has applied attributes and filters."""
  root = checkout(root, expected_source, clean=False)
  allowed = trusted_artifacts(root, expected_source)
  stamp_data, stamp_metadata = regular_file(root, STAMP_PATH)
  stamp = read_json(stamp_data)
  if stamp.get("schema") != "sp-linux-device-build/1" or stamp_metadata["mode"] != "100644":
    fail("Invalid provenance stamp")
  if any(not isinstance(stamp.get(key), str) or HASH_RE.fullmatch(stamp[key]) is None
         for key in ("manifest_sha256", "archive_sha256")):
    fail("Provenance stamp is missing its manifest/archive hashes")
  artifacts = validate_manifest({**stamp, "schema": 1}, expected_source, allowed, include_prebuilt=True)
  changed = {name.decode() for name in git(root, "diff", "--cached", "--no-renames", "--name-only", "-z", "--").split(b"\0") if name}
  if STAMP_PATH not in changed or not changed <= allowed | {STAMP_PATH}:
    fail("Staged changes must contain only validated artifacts and the provenance stamp")
  staged = {}
  for entry in git(root, "ls-files", "--stage", "-z", "--").split(b"\0"):
    if not entry:
      continue
    info, name = entry.split(b"\t", 1)
    path = name.decode()
    if path in allowed | {STAMP_PATH}:
      mode, _sha, stage = info.decode().split()
      if stage != "0" or path in staged:
        fail(f"Unmerged artifact in index: {path}")
      staged[path] = mode
  expected = {**artifacts, STAMP_PATH: {"sha256": hashlib.sha256(stamp_data).hexdigest(), "mode": "100644"}}
  for path, metadata in expected.items():
    if staged.get(path) != metadata["mode"]:
      fail(f"Staged artifact mode does not match validated output: {path}")
    if hashlib.sha256(git(root, "show", f":{path}")).hexdigest() != metadata["sha256"]:
      fail(f"Staged artifact bytes do not match validated output: {path}")
  return stamp


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  commands = parser.add_subparsers(dest="command", required=True)
  bundle = commands.add_parser("package")
  bundle.add_argument("--root", required=True, type=Path)
  bundle.add_argument("--manifest", required=True, type=Path)
  bundle.add_argument("--output", required=True, type=Path)
  bundle.add_argument("--source-sha", required=True)
  publish = commands.add_parser("apply")
  publish.add_argument("--root", required=True, type=Path)
  publish.add_argument("--archive", required=True, type=Path)
  publish.add_argument("--expected-source", required=True)
  verify = commands.add_parser("verify-index")
  verify.add_argument("--root", required=True, type=Path)
  verify.add_argument("--expected-source", required=True)
  args = parser.parse_args()
  try:
    if args.command == "package":
      package(args.root, args.manifest, args.output, args.source_sha)
    elif args.command == "apply":
      apply(args.root, args.archive, args.expected_source)
    else:
      verify_index(args.root, args.expected_source)
  except (ValueError, OSError, tarfile.TarError) as error:
    parser.exit(1, f"Artifact verification failed: {error}\n")


if __name__ == "__main__":
  main()
