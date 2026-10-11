"""Fixture tests for mazda-ti/sync/sync_upstream.sh and classify.py on throwaway git repos (no network)."""
import json
import os
import subprocess
import sys
from pathlib import Path

import pytest

SYNC = Path(__file__).resolve().parents[1] / "sync"
BIN = "common/params_pyx.so"
VENDORED = "third_party/lib/vendored.so"


def git(repo, *args):
  return subprocess.run(["git", "-C", str(repo), *args], check=True, capture_output=True, text=True).stdout.strip()


def write(repo, path, data):
  p = Path(repo) / path
  p.parent.mkdir(parents=True, exist_ok=True)
  p.write_bytes(data if isinstance(data, bytes) else data.encode())


def commit(repo, msg):
  git(repo, "add", "-A")
  git(repo, "commit", "-q", "-m", msg)
  return git(repo, "rev-parse", "HEAD")


@pytest.fixture
def world(tmp_path):
  """official history + an integration branch (official + layer + stamped binary) in one repo."""
  r = tmp_path / "repo"
  r.mkdir()
  git(r, "init", "-q", "-b", "official")
  git(r, "config", "user.email", "t@t")
  git(r, "config", "user.name", "t")
  write(r, "common/params_keys.h", "keys = {\n  {\"Upstream\", {PERSISTENT, BOOL}},\n};\n")
  write(r, "common/params.cc", "// params\n")
  write(r, "system/hardware/tici/agnos.json", "{}\n")
  write(r, "opendbc_repo/opendbc/safety/modes/mazda.h", "// stock mazda\n")
  write(r, BIN, b"\x00official-binary\x00")
  write(r, VENDORED, b"\x00vendored-official\x00")
  base = commit(r, "official base")
  git(r, "checkout", "-q", "-b", "integration")
  write(r, "common/params_keys.h", "keys = {\n  {\"Upstream\", {PERSISTENT, BOOL}},\n  {\"TorqueInterceptorEnabled\", {PERSISTENT, BOOL, \"1\", \"1\"}},\n};\n")
  write(r, "opendbc_repo/opendbc/safety/modes/mazda.h", "// mazda-ti safety\n")
  write(r, "mazda-ti/UPSTREAM", base + "\n")
  commit(r, "mazda-ti layer")
  write(r, BIN, b"\x00mazda-ti-built\x00")
  write(r, VENDORED, b"\x00vendored-mazda-ti\x00")
  write(r, "mazda-ti/BUILD-STAMP.json", json.dumps({"artifacts": {BIN: {"sha256": "x", "class": "built-on-device"},
                                                                  VENDORED: {"sha256": "y", "class": "vendored-third-party"}},
                                                    "source_commit": "0" * 40, "source_tree_sha256": "x", "firmware": {}}))
  commit(r, "build: artifacts")
  git(r, "checkout", "-q", "official")
  policy = tmp_path / "policy"
  policy.mkdir()
  (policy / "layer-files.txt").write_text("opendbc_repo/opendbc/safety/modes/mazda.h\n")
  (policy / "review-paths.txt").write_text("system/hardware/tici/agnos.json\n.github/workflows/\n")
  (policy / "derived-outputs.txt").write_text("common/*.so\npanda/board/obj/*\n")
  (policy / "required-hunks.txt").write_text('common/params_keys.h\t{"TorqueInterceptorEnabled", {PERSISTENT, BOOL, "1", "1"}},\n')
  global POLICY
  POLICY = policy
  return r, base


def advance_official(r, path="selfdrive/feature.py", content="x = 1\n", msg="official change"):
  git(r, "checkout", "-q", "official")
  write(r, path, content)
  sha = commit(r, msg)
  return sha


def sync(r, upstream, candidate="", pr="none", report=None):
  env = dict(os.environ, REPO=str(r), UPSTREAM_REF=upstream, INTEGRATION_REF="integration", POLICY_DIR=str(POLICY),
             CANDIDATE_REF=candidate, PR_STATE=pr, REPORT=str(report or (r.parent / "report.md")))
  p = subprocess.run(["bash", str(SYNC / "sync_upstream.sh")], env=env, capture_output=True, text=True)
  head = git(r, "rev-parse", "HEAD")
  return p.returncode, p.stdout.strip().splitlines()[-1] if p.stdout.strip() else p.stderr, head


def classify(r, candidate, old):
  p = subprocess.run([sys.executable, str(SYNC / "classify.py"), "--repo", str(r), "--candidate", candidate,
                      "--policy", str(POLICY), "--from", old, "--report", str(r.parent / "classify.md")],
                     capture_output=True, text=True, check=True)
  return p.stdout


def test_up_to_date(world):
  r, base = world
  rc, status, _ = sync(r, base)
  assert (rc, status) == (0, "UP_TO_DATE")


def test_clean_merge_needs_device_rebuild(world):
  r, base = world
  new = advance_official(r)
  rc, status, head = sync(r, new)
  assert (rc, status) == (0, "MERGED")
  assert git(r, "show", f"{head}:mazda-ti/UPSTREAM") == new
  assert git(r, "merge-base", "--is-ancestor", "integration", head) == ""
  out = classify(r, head, base)
  assert "VERDICT=NEEDS_DEVICE_REBUILD" in out


def test_dependency_only_change_needs_device_rebuild(world):
  r, base = world
  new = advance_official(r, "common/params.cc", "// params v2\n")
  rc, status, head = sync(r, new)
  assert status == "MERGED"
  assert "VERDICT=NEEDS_DEVICE_REBUILD" in classify(r, head, base)


def test_source_conflict_stops(world):
  r, _ = world
  new = advance_official(r, "common/params_keys.h", "keys = {\n  {\"Upstream\", {PERSISTENT, INT}},\n};\n")
  rc, status, _ = sync(r, new)
  assert (rc, status) == (11, "SOURCE_CONFLICT")


def test_derived_binary_conflict_keeps_stamped_build_and_stays_not_ready(world):
  r, base = world
  new = advance_official(r, BIN, b"\x00official-binary-v2\x00", "official build")
  rc, status, head = sync(r, new)
  assert (rc, status) == (0, "MERGED")
  assert subprocess.run(["git", "-C", str(r), "show", f"{head}:{BIN}"], capture_output=True).stdout == b"\x00mazda-ti-built\x00"
  assert "VERDICT=NEEDS_DEVICE_REBUILD" in classify(r, head, base)


def test_layer_file_change_needs_manual_review(world):
  r, base = world
  new = advance_official(r, "opendbc_repo/opendbc/safety/modes/mazda.h", "// stock mazda\n// upstream tweak\n")
  rc, status, head = sync(r, new)
  if status == "MERGED":
    assert "VERDICT=MANUAL_REVIEW" in classify(r, head, base)
  else:
    assert status == "SOURCE_CONFLICT"


def test_agnos_change_needs_manual_review(world):
  r, base = world
  new = advance_official(r, "system/hardware/tici/agnos.json", "{\"version\": 2}\n")
  _, status, head = sync(r, new)
  assert status == "MERGED"
  assert "VERDICT=MANUAL_REVIEW" in classify(r, head, base)


def test_history_rewrite_stops(world):
  r, base = world
  git(r, "checkout", "-q", "--orphan", "rewritten")
  write(r, "README", "rewritten official\n")
  new = commit(r, "rewritten root")
  rc, status, _ = sync(r, new)
  assert (rc, status) == (10, "HISTORY_REWRITTEN")


def test_second_run_extends_open_candidate_fast_forward(world):
  r, _ = world
  first = advance_official(r)
  _, status, c1 = sync(r, first)
  assert status == "MERGED"
  second = advance_official(r, "selfdrive/feature2.py", "y = 2\n", "official change 2")
  _, status, c2 = sync(r, second, candidate=c1, pr="open")
  assert status == "MERGED"
  assert git(r, "merge-base", "--is-ancestor", c1, c2) == ""      # fast-forward from the pushed candidate


def test_merged_pr_restarts_from_integration(world):
  r, _ = world
  first = advance_official(r)
  _, _, c1 = sync(r, first)
  git(r, "checkout", "-q", "integration")
  git(r, "merge", "-q", "--ff-only", c1)                          # the PR was merged
  second = advance_official(r, "selfdrive/feature2.py", "y = 2\n", "official change 2")
  _, status, c2 = sync(r, second, candidate=c1, pr="merged")
  assert status == "MERGED"
  assert git(r, "merge-base", "--is-ancestor", "integration", c2) == ""
  assert git(r, "merge-base", "--is-ancestor", c1, c2) == ""


def test_closed_unmerged_pr_stops(world):
  r, _ = world
  first = advance_official(r)
  _, _, c1 = sync(r, first)
  second = advance_official(r, "selfdrive/feature2.py", "y = 2\n", "official change 2")
  rc, status, _ = sync(r, second, candidate=c1, pr="closed")
  assert (rc, status) == (13, "CANDIDATE_REJECTED")


def test_vendored_binary_conflict_stops(world):
  r, _ = world
  new = advance_official(r, VENDORED, b"\x00vendored-official-v2\x00", "official vendored update")
  rc, status, _ = sync(r, new)
  assert (rc, status) == (11, "SOURCE_CONFLICT")
