"""Fixture tests for mazda-ti/sync/stamp.py (throwaway git repos and a fixture-local trusted policy)."""
import json
import stat
import subprocess
import sys
from pathlib import Path

import pytest

STAMP = Path(__file__).resolve().parents[1] / "sync" / "stamp.py"
FW = "panda/board/obj/panda.bin.signed"
ELF = "panda/board/obj/panda/main.elf"
BOOT = "panda/board/obj/bootstub.panda.bin"
H7 = ("panda/board/obj/panda_h7.bin.signed", "panda/board/obj/panda_h7/main.elf", "panda/board/obj/bootstub.panda_h7.bin")
NATIVE = ("common/params_pyx.so", "common/libcommon.a", "selfdrive/pandad/pandad")
VENDORED = "third_party/lib/vendored.so"
GITVERSION = "panda/board/obj/gitversion.h"
VERSION = "panda/board/obj/version"
TEMPLATES = {GITVERSION: 'extern const uint8_t gitversion[19];\nconst uint8_t gitversion[19] = "DEV-{sha8}-DEBUG";\n',
             VERSION: "DEV-{sha8}-DEBUG"}


def regenerate(repo, sha8):
  for p, t in TEMPLATES.items():
    write(repo, p, t.format(sha8=sha8).encode())


def git(repo, *args):
  return subprocess.run(["git", "-C", str(repo), *args], check=True, capture_output=True, text=True).stdout.strip()


def write(repo, path, data):
  p = repo / path
  p.parent.mkdir(parents=True, exist_ok=True)
  p.write_bytes(data)


def bin_blob(tag, sha8=None):
  body = b"\x00\x01binary-" + tag.encode() + b"\x00"
  if sha8:
    body += b"DEV-" + sha8.encode() + b"-DEBUG\x00"
  return body


def fake_readelf(tmp_path, name, symbols):
  f = tmp_path / name
  f.write_text("#!/bin/sh\n" + "".join(f"echo '   {i}: 08000000 4 OBJECT GLOBAL DEFAULT 3 {s}'\n" for i, s in enumerate(symbols)))
  f.chmod(f.stat().st_mode | stat.S_IEXEC)
  return f


@pytest.fixture
def repo(tmp_path):
  r = tmp_path / "repo"
  r.mkdir()
  git(r, "init", "-q", "-b", "main")
  git(r, "config", "user.email", "t@t")
  git(r, "config", "user.name", "t")
  write(r, "common/params.cc", b"// params\n")
  for p in (FW, ELF, BOOT, *H7, *NATIVE):
    write(r, p, bin_blob("official-" + p))
  write(r, VENDORED, bin_blob("vendored"))
  regenerate(r, "5925aecd")              # as tracked upstream
  git(r, "add", "-A")
  git(r, "commit", "-q", "-m", "source")
  policy = tmp_path / "policy"
  policy.mkdir()
  (policy / "required-artifacts.txt").write_text("\n".join([FW, ELF, BOOT, *H7, *NATIVE]) + "\n")
  (policy / "binary-classes.txt").write_text("vendored-third-party\tthird_party/*\tvendored\n")
  (policy / "generated-text.json").write_text(json.dumps(TEMPLATES))
  ti = fake_readelf(tmp_path, "readelf-ti", ["torque_interceptor", "gen2", "mazda_rx_hook"])
  return r, policy, ti


def build_and_stamp(r, policy, readelf, fw_sha=None, extra_source_edit=False, skip=()):
  src = git(r, "rev-parse", "HEAD")
  built = [p for p in (FW, ELF, BOOT, *H7, *NATIVE) if p not in skip and (r / p).exists()]
  for p in built:
    is_fw = p.endswith(".bin.signed") or "/bootstub." in p
    write(r, p, bin_blob("built-" + p, (fw_sha or src[:8]) if is_fw else None))
  regenerate(r, src[:8])                  # the build rewrites the tracked version files
  lst = r.parent / "built.txt"
  lst.write_text("\n".join(built) + "\n")
  p = subprocess.run([sys.executable, str(STAMP), "write", "--repo", str(r), "--policy", str(policy), "--source-commit", src,
                      "--built-paths", str(lst), "--out", str(r / "mazda-ti/BUILD-STAMP.json")], capture_output=True, text=True)
  if p.returncode != 0:
    return src, p.stderr
  if extra_source_edit:
    write(r, "common/params.cc", b"// params EDITED inside the artifacts commit\n")
  git(r, "add", "-A")
  git(r, "commit", "-q", "-m", "build: tici artifacts")
  return src, None


def verify(r, policy, readelf, rev="HEAD"):
  p = subprocess.run([sys.executable, str(STAMP), "verify", "--repo", str(r), "--policy", str(policy), "--rev", rev,
                      "--readelf", str(readelf)], capture_output=True, text=True)
  return p.returncode, p.stdout + p.stderr


def edit_stamp(r, fn):
  sp = r / "mazda-ti/BUILD-STAMP.json"
  stamp = json.loads(sp.read_text())
  fn(stamp)
  sp.write_text(json.dumps(stamp))
  git(r, "commit", "-q", "--amend", "-a", "--no-edit")


def test_clean_build_is_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  rc, out = verify(r, policy, ti)
  assert rc == 0 and out.startswith("READY"), out


def test_source_edit_inside_artifacts_commit_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti, extra_source_edit=True)
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "differ from the tree" in out, out


def test_merge_after_build_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  write(r, "selfdrive/new_upstream_feature.py", b"x = 1\n")
  git(r, "add", "-A")
  git(r, "commit", "-q", "-m", "upstream change")
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "device rebuild required" in out, out


def test_upstream_binary_replacement_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  write(r, FW, bin_blob("official-again", "deadbeef"))
  git(r, "commit", "-q", "-am", "upstream build")
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "differs from the stamped build" in out, out


def test_firmware_dev_mismatch_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti, fw_sha="0badc0de")
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "carries DEV-0badc0de" in out, out


def test_candidate_elf_without_ti_symbols_is_not_ready(repo, tmp_path):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  plain = fake_readelf(tmp_path, "readelf-plain", ["mazda_rx_hook"])
  rc, out = verify(r, policy, plain)
  assert rc == 1 and "lacks Mazda TI safety symbols" in out, out


def test_new_unstamped_binary_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  write(r, "selfdrive/new_tool/tool.so", bin_blob("new"))
  git(r, "add", "-A")
  git(r, "commit", "-q", "-m", "new binary")
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "unstamped tracked binary" in out, out


def test_stamp_listing_a_text_source_is_rejected(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  def smuggle(st):
    st["artifacts"]["common/params.cc"] = {"sha256": "0" * 64, "class": "built-on-device"}
  write(r, "common/params.cc", b"// hidden edit\n")
  edit_stamp(r, smuggle)
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "non-binary or missing path" in out, out


def test_no_stamp_is_not_ready(repo):
  r, policy, ti = repo
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "never built" in out, out


@pytest.mark.parametrize("gone", [FW, BOOT, H7[0]])
def test_required_firmware_already_absent_from_source_is_not_ready(repo, gone):
  r, policy, ti = repo
  git(r, "rm", "-q", gone)
  git(r, "commit", "-q", "-m", f"remove {gone}")
  _, err = build_and_stamp(r, policy, ti)
  if err is not None:                      # the writer already refuses
    assert "required artifact not built" in err
    return
  rc, out = verify(r, policy, ti)
  assert rc == 1 and f"required artifact missing from the candidate: {gone}" in out, out


def test_required_firmware_deleted_after_build_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  git(r, "rm", "-q", BOOT)
  git(r, "commit", "-q", "-m", "drop bootstub")
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "required artifact missing" in out, out


def test_unknown_firmware_addition_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  write(r, "panda/board/obj/panda_new.bin.signed", bin_blob("new-fw", "00000000"))
  git(r, "add", "-A")
  git(r, "commit", "-q", "-m", "new firmware")
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "outside the trusted inventory" in out, out


def test_stamp_class_claim_for_unbuilt_binary_must_match_policy(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  edit_stamp(r, lambda st: st["artifacts"][VENDORED].update({"class": "model-artifact"}))
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "not allowed by trusted policy" in out, out


def test_generated_version_regeneration_is_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  assert git(r, "show", f"HEAD:{VERSION}") != "DEV-5925aecd-DEBUG"
  rc, out = verify(r, policy, ti)
  assert rc == 0, out


def test_generated_file_tampered_after_build_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti)
  write(r, VERSION, b"DEV-deadbeef-DEBUG")
  git(r, "commit", "-q", "-am", "tamper")
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "does not match the stamp's source" in out, out


def test_unrelated_source_edit_with_regeneration_is_not_ready(repo):
  r, policy, ti = repo
  build_and_stamp(r, policy, ti, extra_source_edit=True)
  rc, out = verify(r, policy, ti)
  assert rc == 1 and "differ from the tree" in out, out
