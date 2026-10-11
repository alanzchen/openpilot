#!/usr/bin/env python3
"""mazda-ti build stamp: bind every tracked binary to the exact source it was built from.

  stamp.py write  --repo R --policy P --source-commit S --built-paths FILE --out mazda-ti/BUILD-STAMP.json
  stamp.py verify --repo R --policy P [--rev HEAD]   -> READY / NOT_READY (+ reasons); exit 0 only when READY

The policy directory is trusted (the copy on the fork's default branch); nothing it decides is taken from the
candidate or the stamp:
  required-artifacts.txt  every path that must exist and be built on the device (all panda firmware + natives)
  binary-classes.txt      the only allowed classes for tracked binaries the build does not produce
  generated-text.json     tracked text files the build rewrites (exact templates containing the source sha8)

READY requires:
- the candidate's non-artifact source tree == the stamp's recorded tree == a fresh hash of the stamp's source commit;
- every required artifact present and built on the device; every other tracked binary either built on the device
  or matched by a trusted class; no firmware outside the trusted inventory;
- every binary's sha256 equal to the stamp;
- DEV-<sha8> read from each firmware file's own bytes == source commit; TI-only safety symbols read from each
  application image's own ELF.
"""
import argparse
import fnmatch
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile

EMPTY_TREE = "4b825dc642cb6eb9a060e54bf8d69288fbee4904"
STAMP_PATH = "mazda-ti/BUILD-STAMP.json"
FIRMWARE_RE = re.compile(r"^panda/board/obj/(.+\.bin\.signed|bootstub\..+\.bin)$")
APP_IMAGE_RE = re.compile(r"^panda/board/obj/([^/]+)\.bin\.signed$")
DEV_RE = re.compile(rb"DEV-([0-9a-f]{8})-DEBUG")
# present only in the mazda-ti safety (official firmware has the mazda_*_hook functions but neither of these globals)
MAZDA_TI_SYMBOLS = ("torque_interceptor", "gen2")


def git(repo, *args, binary=False):
  out = subprocess.run(["git", "-C", repo, *args], check=True, capture_output=True).stdout
  return out if binary else out.decode()


def policy_lines(policy, name):
  with open(os.path.join(policy, name)) as f:
    return [line.rstrip("\n") for line in f if line.strip() and not line.startswith("#")]


def policy_class(policy, path):
  for line in policy_lines(policy, "binary-classes.txt"):
    cls, glob, _reason = line.split("\t", 2)
    if fnmatch.fnmatchcase(path, glob):
      return cls
  return None


def generated_text(policy):
  with open(os.path.join(policy, "generated-text.json")) as f:
    return json.load(f)


def binary_paths(repo, rev):
  paths = []
  for line in git(repo, "diff", "--numstat", "-z", EMPTY_TREE, rev).split("\0"):
    if line.startswith("-\t-\t"):
      paths.append(line[4:])
  return sorted(paths)


def blob(repo, rev, path):
  return git(repo, "show", f"{rev}:{path}", binary=True)


def elf_symbols(data, readelf):
  with tempfile.NamedTemporaryFile(suffix=".elf") as f:
    f.write(data)
    f.flush()
    out = subprocess.run([readelf, "-s", "--wide", f.name], check=True, capture_output=True, text=True).stdout
  return {line.split()[-1] for line in out.splitlines() if len(line.split()) >= 8}


def source_tree_sha256(repo, rev, artifact_paths):
  skip = set(artifact_paths) | {STAMP_PATH}
  h = hashlib.sha256()
  for entry in git(repo, "ls-tree", "-r", "-z", rev).split("\0"):
    if not entry:
      continue
    meta, path = entry.split("\t", 1)
    if path not in skip:
      h.update(meta.encode() + b"\t" + path.encode() + b"\0")
  return h.hexdigest()


def cmd_write(a):
  built = {line.strip() for line in open(a.built_paths) if line.strip()}
  artifacts, unclassified = {}, []
  for p in binary_paths(a.repo, "HEAD"):
    cls = "built-on-device" if p in built else policy_class(a.policy, p)
    if cls is None:
      unclassified.append(p)
      continue
    with open(os.path.join(a.repo, p), "rb") as f:
      artifacts[p] = {"sha256": hashlib.sha256(f.read()).hexdigest(), "class": cls}
  for p, template in generated_text(a.policy).items():
    with open(os.path.join(a.repo, p), "rb") as f:
      data = f.read()
    if data.decode() != template.format(sha8=a.source_commit[:8]):
      sys.exit(f"refusing to stamp: generated file {p} does not match the source commit")
    artifacts[p] = {"sha256": hashlib.sha256(data).hexdigest(), "class": "generated-text"}
  missing = [p for p in policy_lines(a.policy, "required-artifacts.txt") if artifacts.get(p, {}).get("class") != "built-on-device"]
  if unclassified or missing:
    sys.exit("refusing to stamp:\n" + "\n".join([f"  unclassified binary: {p}" for p in unclassified] +
                                               [f"  required artifact not built: {p}" for p in missing]))
  stamp = {
    "schema": 2,
    "source_commit": a.source_commit,
    "source_tree_sha256": source_tree_sha256(a.repo, a.source_commit, artifacts),
    "artifacts": artifacts,
    "toolchain": json.loads(a.toolchain) if a.toolchain else {},
  }
  os.makedirs(os.path.dirname(os.path.abspath(a.out)), exist_ok=True)
  with open(a.out, "w") as f:
    json.dump(stamp, f, indent=1, sort_keys=True)
    f.write("\n")


def verify(repo, rev, policy, readelf="readelf"):
  reasons = []
  try:
    stamp = json.loads(blob(repo, rev, STAMP_PATH))
  except subprocess.CalledProcessError:
    return ["no build stamp: tici artifacts were never built for this tree"]
  src = stamp["source_commit"]
  try:
    git(repo, "merge-base", "--is-ancestor", src, rev)
  except subprocess.CalledProcessError:
    reasons.append(f"stamp source commit {src[:12]} is missing from this history")

  artifacts = stamp["artifacts"]
  current = binary_paths(repo, rev)
  generated = generated_text(policy)
  for p in sorted(set(artifacts) - set(current) - set(generated)):
    reasons.append(f"stamp lists a non-binary or missing path as an artifact: {p}")
  # excluded from the source hash: binaries, plus ONLY the trusted generated text files (checked exactly below)
  excluded = sorted((set(artifacts) & set(current)) | set(generated))
  for p, template in generated.items():
    try:
      data = blob(repo, rev, p)
    except subprocess.CalledProcessError:
      reasons.append(f"generated file missing: {p}")
      continue
    if data.decode(errors="replace") != template.format(sha8=src[:8]):
      reasons.append(f"generated file {p} does not match the stamp's source {src[:8]}")
    if artifacts.get(p, {}).get("sha256") != hashlib.sha256(data).hexdigest():
      reasons.append(f"generated file {p} differs from the stamped build")
  candidate_hash = source_tree_sha256(repo, rev, excluded)
  try:
    built_hash = source_tree_sha256(repo, src, excluded)
  except subprocess.CalledProcessError:
    built_hash = None
  if not (candidate_hash == stamp["source_tree_sha256"] == built_hash):
    reasons.append("non-artifact sources differ from the tree the tici artifacts were built from: device rebuild required")

  required = policy_lines(policy, "required-artifacts.txt")
  for p in required:
    if p not in current:
      reasons.append(f"required artifact missing from the candidate: {p}")
    elif artifacts.get(p, {}).get("class") != "built-on-device":
      reasons.append(f"required artifact not built from this tree: {p}")
  for p in current:
    if p not in artifacts:
      reasons.append(f"unstamped tracked binary: {p}")
      continue
    if hashlib.sha256(blob(repo, rev, p)).hexdigest() != artifacts[p]["sha256"]:
      reasons.append(f"binary differs from the stamped build: {p}")
    if artifacts[p]["class"] != "built-on-device" and artifacts[p]["class"] != policy_class(policy, p):
      reasons.append(f"binary {p} is not built here and not allowed by trusted policy (stamp says {artifacts[p]['class']})")

  required_firmware = [p for p in required if FIRMWARE_RE.match(p)]
  for p in current:
    if FIRMWARE_RE.match(p) and p not in required_firmware:
      reasons.append(f"firmware outside the trusted inventory: {p}")
  for p in required_firmware:
    if p not in current:
      continue
    m = DEV_RE.search(blob(repo, rev, p))
    dev = m.group(1).decode() if m else None
    if dev != src[:8]:
      reasons.append(f"firmware {p} carries DEV-{dev} but the stamp's source is {src[:8]}")
    app = APP_IMAGE_RE.match(p)
    if app:
      elf = f"panda/board/obj/{app.group(1)}/main.elf"
      if elf not in current:
        reasons.append(f"firmware ELF missing: {elf}")
        continue
      missing = [x for x in MAZDA_TI_SYMBOLS if x not in elf_symbols(blob(repo, rev, elf), readelf)]
      if missing:
        reasons.append(f"firmware {p} lacks Mazda TI safety symbols {missing}")
  return reasons


def cmd_verify(a):
  reasons = verify(a.repo, a.rev, a.policy, a.readelf)
  if reasons:
    print("NOT_READY")
    for r in reasons:
      print(f"- {r}")
    sys.exit(1)
  print("READY")


if __name__ == "__main__":
  ap = argparse.ArgumentParser()
  sub = ap.add_subparsers(dest="cmd", required=True)
  w = sub.add_parser("write")
  w.add_argument("--repo", default=".")
  w.add_argument("--policy", required=True)
  w.add_argument("--source-commit", required=True)
  w.add_argument("--built-paths", required=True)
  w.add_argument("--out", default=STAMP_PATH)
  w.add_argument("--toolchain")
  v = sub.add_parser("verify")
  v.add_argument("--repo", default=".")
  v.add_argument("--policy", required=True)
  v.add_argument("--rev", default="HEAD")
  v.add_argument("--readelf", default="readelf")
  a = ap.parse_args()
  {"write": cmd_write, "verify": cmd_verify}[a.cmd](a)
