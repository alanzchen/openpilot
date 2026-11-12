#!/usr/bin/env bash
# Run CPU acceptance tests against freshly built larch64 artifacts in AGNOS.
set -euo pipefail

REPO_ROOT="$(cd "${LINUX_DEVICE_SOURCE:-/work}" && pwd)"
cd "${REPO_ROOT}"
PYTHON="${PYTHON:-python}"
REPORTS_DIR="${1:-${REPO_ROOT}/.linux-device-build/reports}"
mkdir -p "${REPORTS_DIR}"
REPORTS_DIR="$(cd "${REPORTS_DIR}" && pwd)"

CI_TEST_TMP="$(mktemp -d /tmp/starpilot-linux-tests.XXXXXX)"
finish() {
  local runner_status="$?"
  trap - EXIT
  set +e
  "${PYTHON}" - "${REPORTS_DIR}" "${runner_status}" <<'PY'
import json
from pathlib import Path
import sys
import xml.etree.ElementTree as ET

directory = Path(sys.argv[1])
runner_status = int(sys.argv[2])
required = ('params-cpp', 'params-python', 'mazda-safety', 'mazda-interfaces', 'mazda-ti-integration',
            'mazda-longcontrol', 'launch-panda-usb', 'screen-runtime', 'galaxy-settings')
suites = []
for name in required:
  path = directory / (name + '.xml')
  suite = {'name': name, 'report': path.name, 'tests': 0, 'passed': 0, 'failures': 0, 'errors': 0, 'skipped': 0, 'xfailed': 0}
  try:
    cases = list(ET.parse(path).getroot().iter('testcase'))
    suite['tests'] = len(cases)
    for case in cases:
      if case.find('failure') is not None:
        suite['failures'] += 1
      elif case.find('error') is not None:
        suite['errors'] += 1
      elif (skipped := case.find('skipped')) is not None:
        suite['skipped'] += 1
        suite['xfailed'] += skipped.get('type') == 'pytest.xfail'
      else:
        suite['passed'] += 1
    suite['verdict'] = 'PASS' if suite['passed'] > 0 and not suite['failures'] and not suite['errors'] else 'FAIL'
  except (OSError, ET.ParseError) as error:
    suite.update(verdict='FAIL', detail=str(error))
  suites.append(suite)

try:
  native = json.loads((directory / 'native-runtime.json').read_text())
  native_passed = native.get('verdict') == 'PASS'
except (OSError, ValueError):
  native_passed = False
complete = native_passed and all(suite['verdict'] == 'PASS' for suite in suites)
exit_code = runner_status or (0 if complete else 1)
counts = {key: sum(suite[key] for suite in suites) for key in ('tests', 'passed', 'failures', 'errors', 'skipped', 'xfailed')}
report = {'verdict': 'PASS' if exit_code == 0 else 'FAIL', 'exit_code': exit_code,
          'native_runtime': 'PASS' if native_passed else 'FAIL', 'counts': counts, 'suites': suites,
          'not_covered': ['Qualcomm GPU/OpenCL execution', 'physical cameras', 'Panda flashing', 'vehicle behavior']}
(directory / 'tests.json').write_text(json.dumps(report, indent=2) + '\n')
print(f"{report['verdict']}: Linux ARM64 acceptance ({counts['passed']} passed, {counts['failures']} failed, {counts['errors']} errors)")
raise SystemExit(exit_code)
PY
  local receipt_status="$?"
  rm -rf -- "${CI_TEST_TMP}"
  if [[ -n "${CI_MSGQ_DIR:-}" ]]; then
    rm -rf -- "${CI_MSGQ_DIR}"
  fi
  exit "${receipt_status}"
}
trap finish EXIT

# Never let receipts from an earlier run satisfy this run's acceptance gate.
for suite in native-runtime params-cpp params-python mazda-safety mazda-interfaces mazda-ti-integration \
             mazda-longcontrol launch-panda-usb screen-runtime galaxy-settings; do
  rm -f -- "${REPORTS_DIR}/${suite}.xml" "${REPORTS_DIR}/${suite}.json"
done
rm -f -- "${REPORTS_DIR}/tests.json"
export PARAMS_ROOT="${CI_TEST_TMP}/params"
export OPENPILOT_PREFIX="linuxci-$$"
CI_MSGQ_DIR="/dev/shm/msgq_${OPENPILOT_PREFIX}"
export LOG_ROOT="${CI_TEST_TMP}/logs"
export COMMA_CACHE="${CI_TEST_TMP}/downloads"
export SP_HEADLESS_TEST=1
export PYTHONPATH="${REPO_ROOT}:${REPO_ROOT}/starpilot/third_party${PYTHONPATH:+:${PYTHONPATH}}"
# msgq opens endpoint files within this namespace, but does not create its
# parent directory. Normally OpenpilotPrefix's pytest fixture creates it.
mkdir -p "${PARAMS_ROOT}" "${LOG_ROOT}" "${COMMA_CACHE}" "${CI_MSGQ_DIR}"

# Managed FFmpeg is also used by extension modules loaded during acceptance.
FFMPEG_LIB_DIR="$("${PYTHON}" -c 'import ffmpeg; print(ffmpeg.LIB_DIR)')"
export LD_LIBRARY_PATH="${FFMPEG_LIB_DIR}:/usr/local/lib:/system/vendor/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"

"${PYTHON}" - "${REPORTS_DIR}/native-runtime.json" <<'PY' 2>&1 | tee "${REPORTS_DIR}/native-runtime.log"
import importlib
import json
from pathlib import Path
import platform
import struct
import sys
from tempfile import TemporaryDirectory

assert platform.system() == 'Linux' and platform.machine() in ('aarch64', 'arm64'), 'Acceptance requires native Linux ARM64'
assert sys.version_info[:3] == (3, 12, 3), 'Use the Python 3.12.3 runtime shipped by AGNOS 19.6.20'
assert not Path('/TICI').exists(), 'Hardware-free CI must omit /TICI; build with SP_FORCE_TICI=1 instead'
assert Path('/VERSION').read_text().strip() == '19.6.20', 'Acceptance must run in the pinned AGNOS rootfs'

modules = {}
for name in (
  'openpilot.common.params_pyx',
  'openpilot.common.transformations.transformations',
  'msgq.ipc_pyx',
  'msgq.visionipc.visionipc_pyx',
  'openpilot.selfdrive.pandad.pandad_api_impl',
):
  module = importlib.import_module(name)
  path = Path(module.__file__).resolve()
  header = path.read_bytes()[:20]
  assert header[:4] == b'\x7fELF' and header[4:6] == b'\x02\x01', f'{name} is not ELF64 little endian'
  assert struct.unpack_from('<H', header, 18)[0] == 183, f'{name} is not AArch64'
  modules[name] = str(path)

from openpilot.common.params import Params, ParamKeyFlag, ParamKeyType
from openpilot.common.params_pyx import Params as NativeParams
from openpilot.starpilot.common.screen_settings import gaze_dimming_preferences, write_screen_setting

defaults = {
  'ScreenManagement': True,
  'ScreenGazeDimming': False,
  'ScreenGazeDimPercent': 20,
  'ScreenGazeDimDelay': 3,
  'ScreenGazeScreenSide': 0,
  'ScreenGazeYawThreshold': 15,
}
saved = {'ScreenGazeDimPercent': 35, 'ScreenGazeDimDelay': 8, 'ScreenGazeScreenSide': 1, 'ScreenGazeYawThreshold': 25}
with TemporaryDirectory(prefix='native-screen-params-') as directory:
  params = Params(directory)
  assert isinstance(params, NativeParams), 'Python Params fallback cannot satisfy the native registry gate'
  for key, value in defaults.items():
    assert params.get(key, return_default=True) == value, key
    expected_type = ParamKeyType.BOOL if type(value) is bool else ParamKeyType.INT
    assert params.get_type(key) == expected_type, key
    assert params.get_key_flag(key) & ParamKeyFlag.PERSISTENT, key
  for key, value in saved.items():
    write_screen_setting(params, key, value)
  write_screen_setting(params, 'ScreenGazeDimming', True)
  assert gaze_dimming_preferences(params).enabled
  write_screen_setting(params, 'ScreenManagement', False)
  write_screen_setting(params, 'ScreenManagement', True)
  for key, value in saved.items():
    assert params.get(key) == value, key
  write_screen_setting(params, 'ScreenGazeDimming', False)
  write_screen_setting(params, 'ScreenGazeDimming', True)
  assert gaze_dimming_preferences(params).percent == 35

report = {'python': platform.python_version(), 'machine': platform.machine(), 'agnos': '19.6.20',
          'native_modules': modules, 'screen_defaults': defaults, 'verdict': 'PASS'}
Path(sys.argv[1]).write_text(json.dumps(report, indent=2) + '\n')
print('PASS: native ARM64 extensions and persistent screen registry round trips')
PY

# Select the Params cases: the unrelated util suite reads hardware sysfs.
./common/tests/test_common 'params_nonblocking_put,settings_tier_is_independent_from_tuning_level' \
  --reporter junit --out "${REPORTS_DIR}/params-cpp.xml" 2>&1 | tee "${REPORTS_DIR}/params-cpp.log"

run_pytest() {
  local suite="$1"
  shift
  "${PYTHON}" -m pytest --noconftest -c /dev/null -o addopts='' -p no:cacheprovider -p no:randomly -q \
    --junitxml="${REPORTS_DIR}/${suite}.xml" "$@" 2>&1 | tee "${REPORTS_DIR}/${suite}.log"
}

# Params' usual repository fixture imports the manager. Isolate its real native
# stores per test without loading that unrelated hardware/process fixture.
"${PYTHON}" - "${REPORTS_DIR}/params-python.xml" <<'PY' 2>&1 | tee "${REPORTS_DIR}/params-python.log"
import os
import sys

import pytest

class IsolatedParams:
  @pytest.fixture(autouse=True)
  def isolated_params(self, monkeypatch, tmp_path):
    monkeypatch.setenv('PARAMS_ROOT', str(tmp_path / 'params'))
    monkeypatch.setenv('OPENPILOT_PREFIX', os.environ['OPENPILOT_PREFIX'])

raise SystemExit(pytest.main([
  '--noconftest', '-c', '/dev/null', '-o', 'addopts=', '-p', 'no:cacheprovider', '-p', 'no:randomly', '-q',
  '--junitxml=' + sys.argv[1], 'common/tests/test_params.py',
], plugins=[IsolatedParams()]))
PY

# Preserve the prior device-build acceptance coverage, including the explicitly
# marked known Mazda safety findings. Report their xfails in the JUnit output.
run_pytest mazda-safety \
  opendbc_repo/opendbc/safety/tests/test_mazda.py \
  opendbc_repo/opendbc/safety/tests/test_defaults.py \
  opendbc_repo/opendbc/safety/tests/test_mazda_ti.py
run_pytest mazda-interfaces -k 'mazda or MAZDA' \
  opendbc_repo/opendbc/car/tests/test_car_interfaces.py \
  opendbc_repo/opendbc/car/tests/test_fw_fingerprint.py \
  opendbc_repo/opendbc/car/tests/test_can_fingerprint.py \
  opendbc_repo/opendbc/car/tests/test_platform_configs.py \
  opendbc_repo/opendbc/car/tests/test_mazda3_fingerprint.py
# These two files install their own Params test double at import time and must
# each run in a separate Python process, as required by their module docstrings.
run_pytest mazda-ti-integration opendbc_repo/opendbc/car/mazda/tests/test_mazda_ti_integration.py
run_pytest mazda-longcontrol selfdrive/controls/tests/test_longcontrol_mazda_ti.py
run_pytest launch-panda-usb system/updated/tests/test_agnos_init_panda_usb.py

run_pytest screen-runtime \
  starpilot/common/tests/test_screen_*.py \
  selfdrive/ui/tests/test_device_screen_settings.py \
  selfdrive/ui/tests/test_screen_gaze_settings.py \
  selfdrive/ui/tests/test_ui_param_cache.py \
  selfdrive/ui/tests/test_ui_state_performance.py \
  starpilot/system/wheel_controls/tests
run_pytest galaxy-settings \
  starpilot/system/the_galaxy/tests/test_device_settings_frontend.py \
  starpilot/system/the_galaxy/tests/test_device_settings_layout.py \
  starpilot/system/the_galaxy/tests/test_ui_vue_frontend.py \
  starpilot/system/the_galaxy/tests/test_frontend_module_graph.py

printf 'Acceptance reports: %s\n' "${REPORTS_DIR}"
