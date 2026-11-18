#!/usr/bin/env bash
# Host orchestration: native ARM64 builds in independently disposable AGNOS roots.
set -euo pipefail

SOURCE_ROOT="$(git rev-parse --show-toplevel)"
SOURCE_SHA="$(git rev-parse HEAD)"
WORK_ROOT="${LINUX_DEVICE_WORK_ROOT:?Set LINUX_DEVICE_WORK_ROOT outside the checkout}"
IMAGE="${LINUX_DEVICE_IMAGE:-starpilot-agnos-linux-ci:19.6.20}"
JOBS="${LINUX_DEVICE_JOBS:-4}"
BASELINE="967cc94dc9634ae5bb170fd3ea415318e234713a"
test "$(uname -m)" = aarch64
test -z "$(git status --porcelain --untracked-files=no)"
[[ "${JOBS}" =~ ^[1-9][0-9]*$ ]]
mkdir -p "${WORK_ROOT}/reports" "${WORK_ROOT}/control"
WORK_ROOT="$(cd "${WORK_ROOT}" && pwd)"
[[ "${WORK_ROOT}/" != "${SOURCE_ROOT}/"* ]]
CONTROL="${WORK_ROOT}/control"
REPORTS="${WORK_ROOT}/reports"
cp -a tools/linux_device_build/. "${CONTROL}/"
git show "${BASELINE}:build_stamps/mac-larch64.json" > "${CONTROL}/baseline.json"
python3 - "${CONTROL}/baseline.json" "${CONTROL}/artifacts.txt" <<'PY'
import json
from pathlib import Path, PurePosixPath
import sys
paths = sorted(set(json.loads(Path(sys.argv[1]).read_text())['package_inventory']) - {'prebuilt'})
assert paths and all(not PurePosixPath(p).is_absolute() and '..' not in PurePosixPath(p).parts for p in paths)
Path(sys.argv[2]).write_text('\n'.join(paths) + '\n')
PY

python3 "${SOURCE_ROOT}/tools/linux_device_build/environment/prepare_rootfs.py" \
  --cache-dir "${WORK_ROOT}/agnos-cache" --output-dir "${WORK_ROOT}/agnos-context"
docker build --platform linux/arm64 -t "${IMAGE}" "${WORK_ROOT}/agnos-context"
docker image inspect "${IMAGE}" --format '{{.Id}}' > "${REPORTS}/image-id.txt"
docker run --rm --network none "${IMAGE}" cat /opt/linux-device-build/provenance.json > "${REPORTS}/environment.json"
# The verified, extracted image is now an immutable Docker layer. Free duplicate
# download/context bytes before two full project builds.
rm -rf -- "${WORK_ROOT}/agnos-cache" "${WORK_ROOT}/agnos-context"

SOURCE_EPOCH="$(git show -s --format=%ct "${SOURCE_SHA}")"
for attempt in b1 b2; do
  checkout="${WORK_ROOT}/${attempt}"
  mkdir -p "${REPORTS}/${attempt}"
  git clone --no-hardlinks --no-checkout "${SOURCE_ROOT}" "${checkout}"
  git -C "${checkout}" checkout --detach "${SOURCE_SHA}"
  # The host's shallow fetch may retain this disconnected pin only in
  # FETCH_HEAD. Local clones do not copy that unadvertised object, so fetch it
  # explicitly and verify the immutable inventory before spending time building.
  git -C "${checkout}" fetch --no-tags --depth=1 origin "${BASELINE}"
  git -C "${checkout}" show "${BASELINE}:build_stamps/mac-larch64.json" > "${REPORTS}/${attempt}/baseline.json"
  cmp "${CONTROL}/baseline.json" "${REPORTS}/${attempt}/baseline.json"
  # Force every shipped generated/native artifact to be recreated. Do not
  # accept tracked prebuilts or a warm SCons cache as evidence of a build.
  python3 - "${checkout}" "${CONTROL}/artifacts.txt" <<'PY'
from pathlib import Path
import sys
root = Path(sys.argv[1])
for name in Path(sys.argv[2]).read_text().splitlines() + ['prebuilt']:
  path = root / name
  if path.is_symlink():
    raise SystemExit('Refusing symlink build output: ' + name)
  path.unlink(missing_ok=True)
PY
  docker run --rm --network none --read-only --tmpfs /tmp:rw,exec,nosuid,size=4g --shm-size=1g --platform linux/arm64 \
    --mount "type=bind,src=${checkout},dst=/work" \
    --mount "type=bind,src=${CONTROL},dst=/control,readonly" \
    --mount "type=bind,src=${REPORTS}/${attempt},dst=/reports" \
    -e SOURCE_DATE_EPOCH="${SOURCE_EPOCH}" -e PYTHONHASHSEED=0 -e TZ=UTC \
    -e SP_FORCE_TICI=1 -e SP_FORCE_ARCH=larch64 \
    -e SP_BUILD_WARP_ARTIFACTS=0 -e SP_SKIP_DM_TINYGRAD_PKL=1 \
    -e SP_SCONS_CACHE_DIR=/tmp/scons-cache -e XDG_CACHE_HOME=/tmp/cache -e CACHEDB=/tmp/tinygrad-cache.db \
    -e LINUX_DEVICE_SOURCE=/work -e LINUX_DEVICE_JOBS="${JOBS}" \
    "${IMAGE}" bash /control/container_build.sh
done

python3 "${CONTROL}/gates.py" compare \
  --first "${REPORTS}/b1/artifacts.json" --second "${REPORTS}/b2/artifacts.json" \
  --output "${REPORTS}/reproducibility.json"

# Tests use the first independently rebuilt artifact set, never host binaries.
docker run --rm --network none --read-only --tmpfs /tmp:rw,exec,nosuid,size=4g --shm-size=1g --platform linux/arm64 \
  --mount "type=bind,src=${WORK_ROOT}/b1,dst=/work" \
  --mount "type=bind,src=${CONTROL},dst=/control,readonly" \
  --mount "type=bind,src=${REPORTS}/b1,dst=/reports" \
  -e SP_HEADLESS_TEST=1 -e LINUX_DEVICE_SOURCE=/work \
  "${IMAGE}" bash -euo pipefail -c \
  'export GIT_CONFIG_GLOBAL=/tmp/linux-device-gitconfig; git config --global --add safe.directory /work; bash /control/test.sh /reports'

python3 - "${REPORTS}" "${SOURCE_SHA}" <<'PY'
import json
from pathlib import Path
import sys
reports = Path(sys.argv[1])
manifest = json.loads((reports / 'b1/artifacts.json').read_text())
assert manifest['source_sha'] == sys.argv[2]
gates = {}
for name, path in {
  'reproducibility': 'reproducibility.json',
  'abi': 'b1/abi.json', 'params': 'b1/params.json',
  'source-immutability-b1': 'b1/source.json', 'source-immutability-b2': 'b2/source.json',
  'acceptance': 'b1/tests.json',
}.items():
  result = json.loads((reports / path).read_text())
  assert result['verdict'] == 'PASS', name
  gates[name] = result
manifest['gates'] = gates
manifest['environment'] = json.loads((reports / 'environment.json').read_text())
manifest['image_id'] = (reports / 'image-id.txt').read_text().strip()
manifest['not_covered'] = ['Qualcomm GPU/OpenCL execution', 'physical cameras', 'Panda flashing', 'vehicle behavior']
(reports / 'verified-manifest.json').write_text(json.dumps(manifest, indent=2, sort_keys=True) + '\n')
PY
python3 "${CONTROL}/package.py" package --root "${WORK_ROOT}/b1" \
  --manifest "${REPORTS}/verified-manifest.json" --source-sha "${SOURCE_SHA}" \
  --output "${WORK_ROOT}/device-artifacts.tar"
sha256sum "${WORK_ROOT}/device-artifacts.tar" > "${REPORTS}/archive.sha256"
printf 'Verified device artifact bundle: %s\n' "${WORK_ROOT}/device-artifacts.tar"
