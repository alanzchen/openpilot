#!/usr/bin/env bash
# mazda-ti: merge new official StarPilot commits into the autosync candidate, or stop and explain why not.
# Trusted script (authoritative copy lives on the fork's default branch). It never force-pushes, never
# auto-resolves source conflicts, never merges into the integration branch and never deploys.
#
# env: REPO (work tree), UPSTREAM_REF (fetched official tip), INTEGRATION_REF (mazda-starpilot-ti tip),
#      CANDIDATE_REF (existing autosync/starpilot tip, or empty), PR_STATE (open|merged|closed|none),
#      POLICY_DIR (trusted policy files: derived-outputs.txt), REPORT (markdown report path)
# exit: 0 MERGED/UP_TO_DATE, 10 HISTORY_REWRITTEN, 11 SOURCE_CONFLICT, 12 INTEGRATION_CONFLICT, 13 CANDIDATE_REJECTED
set -euo pipefail
REPO=${REPO:-.}
REPORT=${REPORT:-sync-report.md}
PR_STATE=${PR_STATE:-none}
CANDIDATE_REF=${CANDIDATE_REF:-}
cd "$REPO"

note() { echo "- $*" >> "$REPORT"; }
finish() { echo "STATUS=$1" >> "$REPORT"; echo "$1"; exit "$2"; }
rev() { git rev-parse --verify -q "$1^{commit}"; }

: > "$REPORT"
echo "# mazda-ti upstream sync" >> "$REPORT"
upstream=$(rev "$UPSTREAM_REF")
integration=$(rev "$INTEGRATION_REF")
note "official tip: $upstream"
note "integration: $integration"

# 1. Choose the starting point without ever needing a force-push.
start=$integration
if [ -n "$CANDIDATE_REF" ] && candidate=$(rev "$CANDIDATE_REF"); then
  note "existing candidate: $candidate (PR $PR_STATE)"
  if git merge-base --is-ancestor "$candidate" "$integration"; then
    note "previous candidate already merged into integration; restarting from integration (fast-forward)"
  elif [ "$PR_STATE" = "closed" ]; then
    note "previous candidate PR was closed unmerged; delete or rework autosync/starpilot manually"
    finish CANDIDATE_REJECTED 13
  else
    start=$candidate
  fi
fi

# 2. The official branch must only move forward relative to what this line last merged.
prev_upstream=$(git show "$start:mazda-ti/UPSTREAM" 2>/dev/null | tr -d '[:space:]')
note "last merged official: ${prev_upstream:-unknown}"
if [ -z "$prev_upstream" ] || ! git cat-file -e "$prev_upstream^{commit}" 2>/dev/null || \
   ! git merge-base --is-ancestor "$prev_upstream" "$upstream"; then
  note "official history was rewritten (or the recorded base is unknown); re-apply the layer manually"
  finish HISTORY_REWRITTEN 10
fi

git -c advice.detachedHead=false checkout -q --detach "$start"
git config user.name "mazda-ti autosync"
git config user.email "mazda-ti-autosync@users.noreply.github.com"

# 3. Keep an open candidate current with the integration branch (merge, never rebase).
if ! git merge-base --is-ancestor "$integration" HEAD; then
  if ! git merge -q --no-ff --no-edit -m "autosync: merge mazda-ti integration ${integration:0:12}" "$integration"; then
    note "conflict merging integration into the candidate: $(git diff --name-only --diff-filter=U | tr '\n' ' ')"
    git merge --abort
    finish INTEGRATION_CONFLICT 12
  fi
fi

if git merge-base --is-ancestor "$upstream" HEAD; then
  note "official has no commits newer than the candidate"
  [ "$(git rev-parse HEAD)" = "$start" ] && finish UP_TO_DATE 0
  note "candidate: $(git rev-parse HEAD)"
  finish MERGED 0
fi

# 4. Merge official. Only conflicts in stamped derived binaries may keep ours (they must be rebuilt on the
#    device anyway; the build stamp keeps the candidate NOT_READY). Anything else stops the sync.
# keep-ours is limited to paths that are both stamped as built on the device AND trusted rebuildable outputs
stamped=$(git show HEAD:mazda-ti/BUILD-STAMP.json 2>/dev/null | python3 -c '
import fnmatch, json, sys
globs = [l.strip() for l in open(sys.argv[1]) if l.strip() and not l.startswith("#")]
arts = json.load(sys.stdin)["artifacts"]
print("\n".join(sorted(p for p, a in arts.items() if a.get("class") == "built-on-device" and any(fnmatch.fnmatchcase(p, g) for g in globs))))
' "$POLICY_DIR/derived-outputs.txt" 2>/dev/null || true)
if git merge -q --no-ff --no-edit -m "autosync: merge official StarPilot ${upstream:0:12}" "$upstream"; then
  note "merged cleanly"
else
  conflicted=$(git diff --name-only --diff-filter=U | sort -u)
  source_conflicts=$(comm -23 <(echo "$conflicted") <(echo "$stamped") | sed '/^$/d')
  if [ -n "$source_conflicts" ]; then
    note "source conflicts (not auto-resolved): $(echo "$source_conflicts" | tr '\n' ' ')"
    git merge --abort
    finish SOURCE_CONFLICT 11
  fi
  while read -r f; do [ -n "$f" ] && git checkout -q --ours -- "$f" && git add -- "$f"; done <<< "$conflicted"
  git commit -q --no-edit
  note "derived-binary conflicts kept the stamped build, which is now stale: $(echo "$conflicted" | tr '\n' ' ')"
fi
echo "$upstream" > mazda-ti/UPSTREAM
git add mazda-ti/UPSTREAM
git commit -q --amend --no-edit
note "candidate: $(git rev-parse HEAD)"
finish MERGED 0
