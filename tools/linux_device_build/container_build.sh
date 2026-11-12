#!/usr/bin/env bash
set -euo pipefail
cd /work
export GIT_CONFIG_GLOBAL=/tmp/linux-device-gitconfig
git config --global --add safe.directory /work
export PYTHONPATH="/work:/work/opendbc_repo:/work/third_party/acados:${PYTHONPATH:-}"
export LD_LIBRARY_PATH="$(python -c 'import ffmpeg; print(ffmpeg.LIB_DIR)'):/usr/local/lib:/system/vendor/lib64:${LD_LIBRARY_PATH:-}"
export PARAMS_ROOT=/tmp/params
mkdir -p "${PARAMS_ROOT}"
python - <<'PY'
import hashlib
import json
from pathlib import Path
import platform
assert platform.machine() == 'aarch64'
assert platform.python_version() == '3.12.3'
assert Path('/VERSION').read_text().strip() == '19.6.20'
for name, expected in json.loads(Path('/opt/linux-device-build/protected-packages.json').read_text()).items():
  with (Path('/usr/local/venv/lib/python3.12/site-packages') / name).open('rb') as source:
    assert hashlib.file_digest(source, 'sha256').hexdigest() == expected, name
PY
scons -j"${LINUX_DEVICE_JOBS}" --extras --cache-disable 2>&1 | tee /reports/build.log
python /control/gates.py manifest --root /work --artifacts-file /control/artifacts.txt --output /reports/artifacts.json
python /control/gates.py abi --root /work --artifacts-file /control/artifacts.txt --output /reports/abi.json
python /control/gates.py params --root /work --output /reports/params.json
python - <<'PY'
import json
from pathlib import Path
import subprocess
allowed = set(Path('/control/artifacts.txt').read_text().splitlines())
changed = set(subprocess.check_output(['git', 'diff', '--name-only', '-z']).decode().split('\0')) - {''}
unexpected = sorted(changed - allowed)
report = {'verdict': 'FAIL' if unexpected else 'PASS', 'unexpected_source_changes': unexpected,
          'changed_artifact_count': len(changed & allowed)}
Path('/reports/source.json').write_text(json.dumps(report, indent=2) + '\n')
assert not unexpected, unexpected
PY
