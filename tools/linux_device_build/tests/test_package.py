"""Publisher tests use miniature Git checkouts; no device code is executed."""

import hashlib
import io
import json
import subprocess
import tarfile
from types import SimpleNamespace

import pytest

from openpilot.tools.linux_device_build import package as bundle


def run_git(root, *args):
  return subprocess.check_output(["git", "-C", str(root), *args], stderr=subprocess.PIPE).decode().strip()


def commit(root, message):
  run_git(root, "add", ".")
  run_git(root, "-c", "user.name=Test", "-c", "user.email=test@example.invalid", "commit", "-m", message)
  return run_git(root, "rev-parse", "HEAD")


def metadata(path):
  return {"sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
          "mode": "100755" if path.stat().st_mode & 0o111 else "100644"}


@pytest.fixture
def build(tmp_path, monkeypatch):
  root = tmp_path / "build"
  root.mkdir()
  run_git(root, "init")
  files = {"common/params_pyx.so": b"old native artifact", "cereal/services.h": b"old generated header", "prebuilt": b""}
  for relative, content in files.items():
    path = root / relative
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(content)
    path.chmod(0o755 if relative.endswith(".so") else 0o644)
  source = root / "common/params.py"
  source.write_text("# source must never be replaced by the publisher\n")
  stamp = root / bundle.BASELINE_INVENTORY
  stamp.parent.mkdir()
  stamp.write_text(json.dumps({"package_inventory": {path: metadata(root / path) for path in files}}))
  baseline = commit(root, "baseline")
  monkeypatch.setattr(bundle, "BASELINE_SHA", baseline)
  source.write_text("# exact candidate source\n")
  sha = commit(root, "candidate source")
  publisher = tmp_path / "publisher"
  subprocess.run(["git", "clone", "--no-hardlinks", str(root), str(publisher)], check=True, capture_output=True)
  (root / "common/params_pyx.so").write_bytes(b"new reproducible native artifact")
  (root / "cereal/services.h").write_bytes(b"new generated header")
  manifest = {"schema": 1, "verdict": "PASS", "source_sha": sha,
              "artifacts": {path: metadata(root / path) for path in files if path != "prebuilt"}}
  manifest_path = tmp_path / "gates.json"
  manifest_path.write_bytes(bundle.canonical_json(manifest))
  archive = tmp_path / "artifacts.tar"
  return SimpleNamespace(root=root, publisher=publisher, sha=sha, manifest=manifest,
                         manifest_path=manifest_path, archive=archive, outside=tmp_path)


def create_archive(build):
  build.manifest_path.write_bytes(bundle.canonical_json(build.manifest))
  return bundle.package(build.root, build.manifest_path, build.archive, build.sha)


def archive_members(path):
  with tarfile.open(path, "r:") as archive:
    return [(member, archive.extractfile(member).read()) for member in archive]


def replace_archive(path, members):
  with tarfile.open(path, "w", format=tarfile.USTAR_FORMAT) as archive:
    for member, data in members:
      member.size = len(data)
      archive.addfile(member, io.BytesIO(data) if member.isreg() else None)


def assert_clean_publisher(build):
  assert run_git(build.publisher, "status", "--porcelain", "--untracked-files=all") == ""
  assert (build.publisher / "common/params.py").read_text() == "# exact candidate source\n"


def test_roundtrip_publishes_only_artifacts_and_bound_provenance(build):
  create_archive(build)
  stamp = bundle.apply(build.publisher, build.archive, build.sha)
  assert stamp["schema"] == "sp-linux-device-build/1"
  assert stamp["source_sha"] == build.sha
  assert stamp["archive_sha256"] == hashlib.sha256(build.archive.read_bytes()).hexdigest()
  assert stamp["artifacts"]["prebuilt"] == bundle.PREBUILT
  assert (build.publisher / "common/params_pyx.so").read_bytes() == (build.root / "common/params_pyx.so").read_bytes()
  assert (build.publisher / "common/params_pyx.so").stat().st_mode & 0o777 == 0o755
  assert (build.publisher / "prebuilt").read_bytes() == b""
  changed = set(run_git(build.publisher, "diff", "--name-only").splitlines())
  assert changed == {"common/params_pyx.so", "cereal/services.h"}
  assert json.loads((build.publisher / bundle.STAMP_PATH).read_text()) == stamp
  assert (build.publisher / "common/params.py").read_text() == "# exact candidate source\n"


def test_packaging_is_byte_reproducible(build):
  create_archive(build)
  first = build.archive.read_bytes()
  create_archive(build)
  assert build.archive.read_bytes() == first


def test_gate_evidence_is_preserved_in_archive_and_provenance(build):
  evidence = {"environment": {"verdict": "PASS", "python": "3.12.3", "agnos": "19.6.20"},
              "tests": {"verdict": "PASS", "passed": 120}, "reproducibility": {"verdict": "PASS"}}
  build.manifest["gates"] = evidence
  create_archive(build)
  stamp = bundle.apply(build.publisher, build.archive, build.sha)
  assert stamp["gates"] == evidence


def test_packaging_rejects_source_edits_even_with_valid_artifact_hashes(build):
  (build.root / "common/params.py").write_text("unvalidated source mutation")
  with pytest.raises(ValueError, match="outside the artifact"):
    create_archive(build)
  assert not build.archive.exists()


@pytest.mark.parametrize("path", ["../outside", "/absolute", ".git/config", "common/../params.py", "common//params.py", "common/params.py"])
def test_manifest_cannot_expand_writable_paths(build, path):
  build.manifest["artifacts"][path] = dict(bundle.PREBUILT)
  with pytest.raises(ValueError, match="inventory"):
    create_archive(build)
  assert not build.archive.exists()


@pytest.mark.parametrize("failure", ["missing", "hash", "mode", "bad_mode_type", "verdict", "source", "schema"])
def test_packaging_requires_complete_passing_manifest_and_exact_bytes(build, failure):
  artifact = build.manifest["artifacts"]["common/params_pyx.so"]
  if failure == "missing":
    del build.manifest["artifacts"]["cereal/services.h"]
  elif failure == "hash":
    artifact["sha256"] = "0" * 64
  elif failure == "mode":
    artifact["mode"] = "100644"
  elif failure == "bad_mode_type":
    artifact["mode"] = []
  elif failure == "verdict":
    build.manifest["verdict"] = "FAIL"
  elif failure == "source":
    build.manifest["source_sha"] = "0" * 40
  else:
    build.manifest["schema"] = True
  with pytest.raises(ValueError):
    create_archive(build)
  assert not build.archive.exists()


def test_candidate_cannot_replace_the_immutable_baseline_allowlist(build):
  path = build.root / bundle.BASELINE_INVENTORY
  forged = json.loads(path.read_text())
  forged["package_inventory"]["common/params.py"] = metadata(build.root / "common/params.py")
  path.write_text(json.dumps(forged))
  build.sha = commit(build.root, "candidate modifies local inventory")
  build.manifest["source_sha"] = build.sha
  build.manifest["artifacts"]["common/params.py"] = metadata(build.root / "common/params.py")
  with pytest.raises(ValueError, match="inventory"):
    create_archive(build)


@pytest.mark.parametrize("target", ["file", "parent"])
def test_packaging_rejects_symlink_artifact_or_parent(build, target):
  if target == "file":
    path = build.root / "common/params_pyx.so"
    outside = build.outside / "external.so"
    outside.write_bytes(path.read_bytes())
    path.unlink()
    path.symlink_to(outside)
  else:
    path = build.root / "cereal"
    outside = build.outside / "external-directory"
    path.rename(outside)
    path.symlink_to(outside, target_is_directory=True)
  with pytest.raises((OSError, ValueError)):
    create_archive(build)


def test_packaging_cannot_overwrite_source_with_archive(build):
  with pytest.raises(ValueError, match="outside"):
    bundle.package(build.root, build.manifest_path, build.root / "common/params.py", build.sha)
  assert (build.root / "common/params.py").read_text() == "# exact candidate source\n"


@pytest.mark.parametrize("kind", ["symlink", "hardlink", "duplicate", "source", "traversal", "mode", "hash", "missing"])
def test_malicious_archives_fail_before_any_checkout_mutation(build, kind):
  create_archive(build)
  members = archive_members(build.archive)
  artifact = next(index for index, (member, _data) in enumerate(members) if member.name == "artifacts/common/params_pyx.so")
  member, data = members[artifact]
  if kind in ("symlink", "hardlink"):
    member.type = tarfile.SYMTYPE if kind == "symlink" else tarfile.LNKTYPE
    member.linkname = "../../outside"
    members[artifact] = member, b""
  elif kind == "duplicate":
    members.append((tarfile.TarInfo(member.name), b"duplicate"))
  elif kind in ("source", "traversal"):
    extra = tarfile.TarInfo("artifacts/common/params.py" if kind == "source" else "../outside")
    extra.mode = 0o644
    members.append((extra, b"bad source"))
  elif kind == "mode":
    member.mode = 0o644
  elif kind == "hash":
    members[artifact] = member, b"tampered artifact"
  else:
    members.pop(artifact)
  replace_archive(build.archive, members)
  with pytest.raises((ValueError, tarfile.TarError)):
    bundle.apply(build.publisher, build.archive, build.sha)
  assert_clean_publisher(build)


def test_manifest_source_binding_is_enforced_by_publisher(build):
  create_archive(build)
  members = archive_members(build.archive)
  for index, (member, data) in enumerate(members):
    if member.name == bundle.CONTROL_NAME:
      manifest = json.loads(data)
      manifest["source_sha"] = "0" * 40
      members[index] = member, bundle.canonical_json(manifest)
  replace_archive(build.archive, members)
  with pytest.raises(ValueError, match="source SHA"):
    bundle.apply(build.publisher, build.archive, build.sha)
  assert_clean_publisher(build)


def test_publisher_rejects_wrong_head_and_dirty_checkout(build):
  create_archive(build)
  with pytest.raises(ValueError, match="HEAD"):
    bundle.apply(build.publisher, build.archive, "0" * 40)
  (build.publisher / "untracked.txt").write_text("publisher state")
  with pytest.raises(ValueError, match="clean"):
    bundle.apply(build.publisher, build.archive, build.sha)


def test_duplicate_manifest_keys_are_rejected():
  with pytest.raises(ValueError, match="Duplicate"):
    bundle.read_json(b'{"schema":1,"schema":1}')


@pytest.mark.parametrize("number", ["NaN", "Infinity", "-Infinity", "1e999", "-1e999"])
def test_nonfinite_extra_manifest_metadata_is_rejected_before_any_write(build, number):
  create_archive(build)
  members = archive_members(build.archive)
  for index, (member, data) in enumerate(members):
    if member.name == bundle.CONTROL_NAME:
      members[index] = member, data.rstrip()[:-1] + b', "extra": ' + number.encode() + b'}\n'
  replace_archive(build.archive, members)
  with pytest.raises(ValueError):
    bundle.apply(build.publisher, build.archive, build.sha)
  assert_clean_publisher(build)


def stage_bundle(build):
  run_git(build.publisher, "add", "-u")
  run_git(build.publisher, "add", bundle.STAMP_PATH)


def test_verified_index_binds_committed_blobs_to_passed_artifacts(build):
  create_archive(build)
  stamp = bundle.apply(build.publisher, build.archive, build.sha)
  stage_bundle(build)
  assert bundle.verify_index(build.publisher, build.sha) == stamp


@pytest.mark.parametrize("change", ["artifact_bytes", "artifact_mode", "source", "stamp_bytes", "unstaged_stamp"])
def test_publisher_rejects_staging_filters_or_other_index_modifications(build, change):
  create_archive(build)
  bundle.apply(build.publisher, build.archive, build.sha)
  stage_bundle(build)
  if change == "artifact_bytes":
    (build.publisher / "common/params_pyx.so").write_bytes(b"filter transformed native artifact")
    run_git(build.publisher, "add", "common/params_pyx.so")
  elif change == "artifact_mode":
    run_git(build.publisher, "update-index", "--chmod=-x", "common/params_pyx.so")
  elif change == "source":
    (build.publisher / "common/params.py").write_text("unrelated source modification")
    run_git(build.publisher, "add", "common/params.py")
  elif change == "stamp_bytes":
    stamp = build.publisher / bundle.STAMP_PATH
    original = stamp.read_bytes()
    stamp.write_bytes(original + b" ")
    run_git(build.publisher, "add", bundle.STAMP_PATH)
    stamp.write_bytes(original)
  else:
    run_git(build.publisher, "reset", "--", bundle.STAMP_PATH)
  with pytest.raises(ValueError):
    bundle.verify_index(build.publisher, build.sha)
