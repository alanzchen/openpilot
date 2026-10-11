#!/usr/bin/env python3
"""mazda-ti candidate classification (trusted copy runs from the fork's default branch).

  classify.py --repo R --candidate SHA --policy DIR [--from OLD_UPSTREAM] [--report out.md]

Verdicts (most severe wins): MANUAL_REVIEW > NEEDS_DEVICE_REBUILD > READY.
- MANUAL_REVIEW: official changed a layer-owned file or a review path (AGNOS pin, launch scripts, workflows),
  or a required adaptation hunk is missing from the candidate.
- NEEDS_DEVICE_REBUILD: the build stamp does not verify for this exact SHA (stamp.py verify).
- READY: none of the above. READY never deploys anything; merging and installing stay manual.
"""
import argparse
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent


def git(repo, *args):
  return subprocess.run(["git", "-C", repo, *args], check=True, capture_output=True, text=True).stdout


def policy_lines(path):
  return [line.rstrip("\n") for line in open(path) if line.strip() and not line.startswith("#")]


def main():
  ap = argparse.ArgumentParser()
  ap.add_argument("--repo", default=".")
  ap.add_argument("--candidate", required=True)
  ap.add_argument("--policy", required=True)
  ap.add_argument("--from", dest="old_upstream")
  ap.add_argument("--report", default="classify-report.md")
  a = ap.parse_args()

  findings, verdict = [], "READY"
  new_upstream = git(a.repo, "show", f"{a.candidate}:mazda-ti/UPSTREAM").strip()
  if a.old_upstream and a.old_upstream != new_upstream:
    changed = git(a.repo, "diff", "--name-only", a.old_upstream, new_upstream).split()
    layer = set(policy_lines(f"{a.policy}/layer-files.txt"))
    review = policy_lines(f"{a.policy}/review-paths.txt")
    for p in changed:
      if p in layer:
        findings.append(f"MANUAL_REVIEW: official changed layer-owned file {p}")
      if any(p == r or (r.endswith("/") and p.startswith(r)) for r in review):
        findings.append(f"MANUAL_REVIEW: official changed review path {p}")
  for line in policy_lines(f"{a.policy}/required-hunks.txt"):
    path, needle = line.split("\t", 1)
    try:
      content = git(a.repo, "show", f"{a.candidate}:{path}")
    except subprocess.CalledProcessError:
      content = ""
    if needle not in content:
      findings.append(f"MANUAL_REVIEW: required adaptation missing in {path}: {needle!r}")
  if any(f.startswith("MANUAL_REVIEW") for f in findings):
    verdict = "MANUAL_REVIEW"

  stamp = subprocess.run([sys.executable, str(HERE / "stamp.py"), "verify", "--repo", a.repo, "--rev", a.candidate],
                         capture_output=True, text=True)
  if stamp.returncode != 0:
    findings += [f"NEEDS_DEVICE_REBUILD: {r[2:]}" for r in stamp.stdout.splitlines() if r.startswith("- ")]
    if verdict == "READY":
      verdict = "NEEDS_DEVICE_REBUILD"

  with open(a.report, "w") as f:
    f.write(f"# mazda-ti classification for {a.candidate}\n\nVERDICT={verdict}\n\n")
    for x in findings:
      f.write(f"- {x}\n")
  print(f"VERDICT={verdict}")
  for x in findings:
    print(f"- {x}")


if __name__ == "__main__":
  main()
