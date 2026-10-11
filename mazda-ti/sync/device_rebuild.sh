#!/usr/bin/env bash
# mazda-ti: rebuild every scons-produced tracked binary on a comma 3 (tici) from the exact committed source,
# stamp it and commit the artifacts as the direct child of that source commit. Never flashes, installs or
# restarts anything, and never builds in /data/openpilot.
#
# usage: device_rebuild.sh <repo> <branch> <policy-dir> [jobs]
#   Creates a FRESH worktree for <branch> (no untracked or ignored files can influence the build), builds there,
#   commits the artifacts on <branch> and verifies the result against the trusted policy.
set -euo pipefail
REPO=$(cd "$1" && pwd -P)
BRANCH=$2
POLICY=$(cd "$3" && pwd -P)
JOBS=${4:-2}
HERE=$(cd "$(dirname "$0")" && pwd -P)
EMPTY_TREE=4b825dc642cb6eb9a060e54bf8d69288fbee4904

SRC=$(git -C "$REPO" rev-parse --verify "$BRANCH^{commit}")
WT=${BUILD_ROOT:-/data/media/hud-build}/build-${SRC:0:12}
[ ! -e "$WT" ] || { echo "$WT already exists; use a fresh build directory" >&2; exit 2; }
git -C "$REPO" worktree add -q "$WT" "$BRANCH"
cd "$WT"
[ "$WT" != "/data/openpilot" ] || { echo "refusing to build in the deployed tree" >&2; exit 2; }
[ "$(git rev-parse HEAD)" = "$SRC" ] || { echo "branch moved" >&2; exit 2; }
[ -z "$(git status --porcelain --ignored)" ] || { echo "fresh worktree is not pristine" >&2; exit 2; }
echo "building tici artifacts for $SRC"

marker=$(mktemp)
sleep 1   # mtime granularity: everything written by the build is strictly newer than the marker
export PATH=/usr/local/venv/bin:$PATH
# This AGNOS has no system Eigen (the SConstruct expects /usr/include/eigen3); the venv ships it as the `eigen`
# package. SCons only forwards PATH to the compiler, so shim clang/clang++ to add its include dir via CPATH.
EIGEN_INCLUDE=""
if [ ! -d /usr/include/eigen3 ] && [ ! -d /usr/local/include/eigen3 ]; then
  EIGEN_INCLUDE=$(python3 -c 'import eigen; print(eigen.INCLUDE_DIR)')
  SHIM=$(mktemp -d)
  for c in clang clang++; do
    printf '#!/bin/sh\nexport CPATH="%s${CPATH:+:$CPATH}"\nexec %s "$@"\n' "$EIGEN_INCLUDE" "$(command -v $c)" > "$SHIM/$c"
    chmod +x "$SHIM/$c"
  done
  export PATH=$SHIM:$PATH
fi
export EIGEN_INCLUDE
env -u PYTHONPATH PYTHONPATH="$WT" nice -n 15 ionice -c3 scons -j"$JOBS" --minimal --cache-disable \
  || env -u PYTHONPATH PYTHONPATH="$WT" nice -n 15 ionice -c3 scons -j1 --minimal --cache-disable

# The build may only change tracked binaries; any text/source change means the tree is not what we stamp.
mapfile -t binaries < <(git diff --numstat -z "$EMPTY_TREE" HEAD | tr '\0' '\n' | awk -F'\t' '$1=="-" && $2=="-" {print $3}')
# the only tracked text the build may rewrite: trusted generated files (exact content is verified by stamp.py)
generated=$(python3 -c 'import json,sys; print("\n".join(json.load(open(sys.argv[1]))))' "$POLICY/generated-text.json")
changed_text=$(git diff --numstat HEAD | awk -F'\t' '$1!="-" {print $3}' | grep -v -x -F -f <(echo "$generated") || true)
untracked=$(git status --porcelain --untracked-files=all | awk '$1=="??" {print $2}')
if [ -n "$untracked" ]; then
  echo "the build produced untracked, non-ignored files (not stamped, review them):" >&2
  echo "$untracked" >&2
fi
if [ -n "$changed_text" ]; then
  echo "the build modified tracked text files (refusing to stamp):" >&2
  echo "$changed_text" >&2
  exit 3
fi
: > "$WT/.mazda-ti-built-paths"
for p in "${binaries[@]}"; do
  [ "$p" -nt "$marker" ] && echo "$p" >> "$WT/.mazda-ti-built-paths"
done
rm -f "$marker"

toolchain=$(python3 - <<'EOF'
import json, subprocess
def v(cmd):
  try:
    return subprocess.run(cmd, capture_output=True, text=True).stdout.splitlines()[0]
  except Exception:
    return None
print(json.dumps({"gcc": v(["gcc", "--version"]), "clang": v(["clang", "--version"]),
                  "eigen_include_via_cpath_shim": __import__("os").environ.get("EIGEN_INCLUDE") or None,
                  "arm-none-eabi-gcc": v(["arm-none-eabi-gcc", "--version"]),
                  "scons": v(["scons", "--version"]), "python": v(["python3", "--version"]),
                  "agnos": open("/VERSION").read().strip() if __import__("os").path.exists("/VERSION") else None}))
EOF
)
python3 "$HERE/stamp.py" write --repo "$WT" --policy "$POLICY" --source-commit "$SRC" \
  --built-paths "$WT/.mazda-ti-built-paths" --out "$WT/mazda-ti/BUILD-STAMP.json" --toolchain "$toolchain"
rm -f "$WT/.mazda-ti-built-paths"

git add -u
git add mazda-ti/BUILD-STAMP.json
git commit -q -m "build(mazda-ti): tici artifacts for ${SRC:0:12}

Built on a comma 3 (tici) from the committed source ${SRC} with scons --minimal.
See mazda-ti/BUILD-STAMP.json for every tracked binary, its sha256 and origin."
[ "$(git rev-parse HEAD^)" = "$SRC" ] || { echo "artifacts commit parent mismatch" >&2; exit 4; }
python3 "$HERE/stamp.py" verify --repo "$WT" --policy "$POLICY" --rev HEAD
