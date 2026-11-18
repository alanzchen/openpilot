import copy
import json
from dataclasses import replace
from pathlib import Path
from types import SimpleNamespace

import pytest

from openpilot.tools.linux_device_build import gates


SHA = 'a' * 40


def elf(**overrides):
  values = dict(bits=64, machine=183, elf_type=3, needed=[], rpath=[], runpath=[], interpreter=None,
                required_versions={}, defined_versions=set())
  values.update(overrides)
  return gates.ElfInfo(**values)


@pytest.fixture
def device_tree(tmp_path):
  checkout, sysroot = tmp_path / 'checkout', tmp_path / 'agnos'
  checkout.mkdir()
  sysroot.mkdir()
  objects = {}

  def put(path, details):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(b'fake ELF inspected by the fixture')
    objects[path] = details
    return path

  binary = put(checkout / 'common/params_pyx.so', elf(needed=['libfoo.so'], required_versions={'libfoo.so': {'ABI_1'}}))
  library = put(sysroot / 'usr/lib/libfoo.so', elf(defined_versions={'ABI_1'}))

  def check(paths=None):
    return gates.abi_gate(checkout, paths or ['common/params_pyx.so'], sysroot, inspect=objects.get)

  return checkout, sysroot, objects, binary, library, put, check


def test_abi_gate_requires_real_device_arch_and_provider_version_closure(device_tree):
  _, _, _, _, _, _, check = device_tree
  result = check()
  assert result['verdict'] == 'PASS'
  assert len(result['elf_closure']) == 2


@pytest.mark.parametrize('failure', ['missing', 'wrong_arch', 'missing_version', 'non_elf'])
def test_abi_gate_rejects_unloadable_dependency(device_tree, failure):
  _, _, objects, _, library, _, check = device_tree
  if failure == 'missing':
    library.unlink()
  elif failure == 'wrong_arch':
    objects[library] = replace(objects[library], machine=62)
  elif failure == 'missing_version':
    objects[library] = replace(objects[library], defined_versions={'ABI_0'})
  else:
    objects[library] = None
  result = check()
  assert result['verdict'] == 'FAIL'
  assert result['errors']


@pytest.mark.parametrize('bits,machine', [(64, 62), (32, 40), (64, 40)])
def test_abi_gate_rejects_host_or_arm32_native_extension(device_tree, bits, machine):
  _, _, objects, binary, _, _, check = device_tree
  objects[binary] = replace(objects[binary], bits=bits, machine=machine)
  assert check()['verdict'] == 'FAIL'


def test_panda_firmware_is_arm32_and_must_not_require_dynamic_loader(device_tree):
  checkout, _, objects, _, _, put, check = device_tree
  firmware = put(checkout / 'panda/board/obj/panda/main.elf', elf(bits=32, machine=40, elf_type=2))
  paths = ['common/params_pyx.so', 'panda/board/obj/panda/main.elf']
  assert check(paths)['verdict'] == 'PASS'
  objects[firmware] = replace(objects[firmware], needed=['libc.so.6'])
  assert check(paths)['verdict'] == 'FAIL'


@pytest.mark.parametrize('search_path', ['/work/.venv/lib', '/workspace/lib', '/build/openpilot/lib', '.', '', '$LIB/custom'])
def test_abi_gate_rejects_build_mount_or_cwd_loader_paths(device_tree, search_path):
  _, _, objects, binary, _, _, check = device_tree
  objects[binary] = replace(objects[binary], runpath=[search_path])
  assert check()['verdict'] == 'FAIL'


def test_origin_paths_and_absolute_target_symlinks_resolve_in_sysroot(device_tree):
  _, sysroot, objects, binary, library, put, check = device_tree
  actual = put(sysroot / 'opt/device/lib/libfoo-real.so', elf(needed=['libbar.so'], runpath=['$ORIGIN'], defined_versions={'ABI_1'}))
  library.unlink()
  library.symlink_to('/opt/device/lib/libfoo-real.so')
  put(sysroot / 'opt/device/lib/libbar.so', elf())
  result = check()
  assert result['verdict'] == 'PASS'
  assert result['elf_closure'][str(binary)]['needed']['libfoo.so'] == str(actual)
  assert objects[actual].runpath == ['$ORIGIN']


@pytest.mark.parametrize('kind,verdict', [('rpath', 'PASS'), ('runpath', 'FAIL')])
def test_runpath_is_not_inherited_by_transitive_dependencies(device_tree, kind, verdict):
  _, sysroot, objects, binary, library, put, check = device_tree
  library.unlink()
  objects[binary] = replace(objects[binary], **{kind: ['/opt/device/lib']})
  put(sysroot / 'opt/device/lib/libfoo.so', elf(needed=['libbar.so'], defined_versions={'ABI_1'}))
  put(sysroot / 'opt/device/lib/libbar.so', elf())
  assert check()['verdict'] == verdict


def test_device_interpreter_is_validated_and_checked_recursively(device_tree):
  _, sysroot, objects, binary, _, put, check = device_tree
  objects[binary] = replace(objects[binary], interpreter='/lib/ld-linux-aarch64.so.1')
  assert check()['verdict'] == 'FAIL'
  put(sysroot / 'lib/ld-linux-aarch64.so.1', elf())
  assert check()['verdict'] == 'PASS'
  objects[binary] = replace(objects[binary], interpreter='/lib64/ld-linux-x86-64.so.2')
  assert check()['verdict'] == 'FAIL'


def test_readelf_parser_keeps_version_needs_and_exports_separate():
  output = '''
    [Requesting program interpreter: /lib/ld-linux-aarch64.so.1]
    0x0000000000000001 (NEEDED) Shared library: [libc.so.6]
    0x000000000000001d (RUNPATH) Library runpath: [$ORIGIN:/usr/local/lib]
Version definition section '.gnu.version_d' contains 1 entry:
  0x0010: Rev: 1 Flags: none Index: 2 Cnt: 1 Name: OWN_1
Version needs section '.gnu.version_r' contains 1 entry:
  000000: Version: 1 File: libc.so.6 Cnt: 2
  0x0010: Name: GLIBC_2.17 Flags: none Version: 3
  0x0020: Name: GLIBC_2.38 Flags: none Version: 4
'''
  actual = gates.parse_elf_output(output)
  assert actual.needed == ['libc.so.6']
  assert actual.runpath == ['$ORIGIN', '/usr/local/lib']
  assert actual.interpreter == '/lib/ld-linux-aarch64.so.1'
  assert actual.required_versions == {'libc.so.6': {'GLIBC_2.17', 'GLIBC_2.38'}}
  assert actual.defined_versions == {'OWN_1'}


def test_non_little_endian_elf_rejected_before_readelf(tmp_path):
  path = tmp_path / 'extension.so'
  path.write_bytes(b'\x7fELF\x02\x02' + b'\x00' * 14)
  with pytest.raises(gates.GateError, match='little-endian'):
    gates.parse_elf(path)


def test_library_config_directories_are_read_from_target_not_host(tmp_path):
  (tmp_path / 'etc/ld.so.conf.d').mkdir(parents=True)
  (tmp_path / 'etc/ld.so.conf').write_text('include /etc/ld.so.conf.d/*.conf\n')
  (tmp_path / 'etc/ld.so.conf.d/vendor.conf').write_text('# comment\n/opt/vendor/lib # comment\n')
  assert '/opt/vendor/lib' in gates.library_directories(tmp_path)


def test_manifest_compares_exact_bytes_modes_and_sets_without_normalizing_gitversion(tmp_path):
  binary = tmp_path / 'binary.so'
  version = tmp_path / 'gitversion.h'
  binary.write_bytes(b'first build')
  version.write_text('same source version')
  binary.chmod(0o755)
  version.chmod(0o644)
  first = gates.artifact_manifest(tmp_path, ['binary.so', 'gitversion.h'], SHA)
  assert gates.compare_manifests(first, copy.deepcopy(first))['verdict'] == 'PASS'
  version.write_text('nondeterministic source version')
  second = gates.artifact_manifest(tmp_path, ['binary.so', 'gitversion.h'], SHA)
  result = gates.compare_manifests(first, second)
  assert result['verdict'] == 'FAIL'
  assert result['changed'] == ['gitversion.h']
  assert result['normalizations'] == []
  second['artifacts']['binary.so']['mode'] = '100644'
  assert 'binary.so' in gates.compare_manifests(first, second)['changed']


@pytest.mark.parametrize('change', ['source', 'added', 'removed', 'failed'])
def test_reproducibility_does_not_accept_wrong_source_or_incomplete_builds(change):
  first = {'schema': 1, 'verdict': 'PASS', 'source_sha': SHA, 'artifacts': {'x': {'sha256': 'b' * 64, 'mode': '100644'}}}
  second = copy.deepcopy(first)
  if change == 'source':
    second['source_sha'] = 'c' * 40
  elif change == 'added':
    second['artifacts']['y'] = first['artifacts']['x']
  elif change == 'removed':
    first['artifacts']['y'] = first['artifacts']['x']
  else:
    second['verdict'] = 'FAIL'
  if change == 'failed':
    with pytest.raises(gates.GateError):
      gates.compare_manifests(first, second)
  else:
    assert gates.compare_manifests(first, second)['verdict'] == 'FAIL'


def test_manifest_rejects_missing_artifact_symlink_and_unsafe_permissions(tmp_path):
  with pytest.raises(gates.GateError):
    gates.artifact_manifest(tmp_path, ['missing'], SHA)
  (tmp_path / 'file').write_text('artifact')
  (tmp_path / 'link').symlink_to('file')
  with pytest.raises(gates.GateError):
    gates.artifact_manifest(tmp_path, ['link'], SHA)
  (tmp_path / 'file').chmod(0o777)
  with pytest.raises(gates.GateError):
    gates.artifact_manifest(tmp_path, ['file'], SHA)


def test_baseline_inventory_excludes_only_control_marker_and_checks_paths(tmp_path):
  baseline = tmp_path / gates.BASELINE
  baseline.parent.mkdir()
  baseline.write_text(json.dumps({'package_inventory': {'prebuilt': {}, 'common/params_pyx.so': {}}}))
  assert gates.artifact_paths(tmp_path) == ['common/params_pyx.so']
  inventory = tmp_path / 'paths.txt'
  inventory.write_text('../escape\n')
  with pytest.raises(gates.GateError):
    gates.artifact_paths(tmp_path, inventory)


def test_source_registry_parser_handles_json_and_schema_derived_default(tmp_path):
  (tmp_path / 'common').mkdir()
  (tmp_path / 'cereal').mkdir()
  registry = tmp_path / 'common/params_keys.h'
  registry.write_text('''
    {"Setting", {PERSISTENT | DONT_LOG, JSON, "{\\"x\\":[1,2]}", std::nullopt, 2, SETTINGS_SIMPLE}},
    {"Personality", {PERSISTENT, INT, std::to_string(static_cast<int>(cereal::LongitudinalPersonality::STANDARD))}}, // a source comment
''')
  (tmp_path / 'cereal/log.capnp').write_text('enum LongitudinalPersonality { aggressive @0; standard @1; relaxed @2; }')
  entries = gates.registry_source(registry)
  assert entries['Setting']['default'] == '{"x":[1,2]}'
  assert entries['Setting']['flags'] == ['PERSISTENT ', ' DONT_LOG']
  assert entries['Setting']['tier'] == 'SETTINGS_SIMPLE'
  assert entries['Personality']['default'] == '1'


def test_real_source_registry_and_shipping_inventory_are_parseable():
  root = Path(__file__).resolve().parents[2]
  registry = gates.registry_source(root / 'common/params_keys.h')
  assert len(registry) == 808
  assert 'RecordFrontLock' in registry
  assert registry['ScreenGazeDimming']['default'] == '0'
  assert registry['ScreenGazeDimmingMode']['type'] == 'INT'
  assert registry['ScreenGazeDimmingMode']['default'] == '0'
  assert len(gates.artifact_paths(root)) == 168


def test_params_command_fails_closed_on_host_and_writes_report(tmp_path, monkeypatch, capsys):
  monkeypatch.setattr(gates.platform, 'machine', lambda: 'x86_64')
  output = tmp_path / 'params.json'
  assert gates.main(['params', '--root', str(tmp_path), '--output', str(output)]) == 1
  report = json.loads(output.read_text())
  assert report['verdict'] == 'FAIL'
  assert 'native ARM64' in report['errors'][0]
  assert report['errors'][0] in capsys.readouterr().err


@pytest.mark.parametrize('defect', [None, 'missing_gaze_key', 'missing_mode_key', 'wrong_mode_default', 'wrong_mode_type',
                                  'mode_cleared_on_toggle', 'mode_restore_failed',
                                  'wrong_default', 'wrong_type', 'wrong_flags', 'not_persistent',
                                  'legacy_bool_stock', 'legacy_string_stock', 'legacy_int_stock', 'legacy_float_stock',
                                  'explicit_empty_stock', 'explicit_false_stock', 'missing_stock_and_default'])
def test_native_registry_gate_catches_stale_registry_and_storage_failure(tmp_path, defect):
  source = {
    'ScreenManagement': {'flags': ['PERSISTENT'], 'type': 'BOOL', 'default': '1', 'stock': '0', 'tuning': 1, 'tier': 'SETTINGS_SIMPLE'},
    'ScreenGazeDimming': {'flags': ['PERSISTENT'], 'type': 'BOOL', 'default': '0', 'stock': '0', 'tuning': 1, 'tier': 'SETTINGS_SIMPLE'},
    **{key: {'flags': ['PERSISTENT'], 'type': 'INT', 'default': default, 'stock': default, 'tuning': 2, 'tier': 'SETTINGS_SIMPLE'}
       for key, default in [('ScreenGazeDimmingMode', '0'), ('ScreenGazeDimPercent', '20'), ('ScreenGazeDimDelay', '3'),
                            ('ScreenGazeScreenSide', '0'), ('ScreenGazeYawThreshold', '15')]},
  }
  # Legacy stock values are optional: the C++ API falls back to the default
  # only for a missing optional, including when that default is false/empty/0.
  for key, kind, default, stock in [
    ('LegacyBool', 'BOOL', '0', None),
    ('LegacyString', 'STRING', 'main_en', None),
    ('LegacyInt', 'INT', '0', None),
    ('LegacyFloat', 'FLOAT', '0.0', None),
    ('ExplicitEmptyStock', 'STRING', 'fallback', ''),
    ('ExplicitFalseStock', 'BOOL', '1', '0'),
    ('MissingStockAndDefault', 'STRING', None, None),
    ('EmptyDefault', 'STRING', '', None),
  ]:
    source[key] = {'flags': ['PERSISTENT'], 'type': kind, 'default': default, 'stock': stock, 'tuning': 0, 'tier': 'SETTINGS_SIMPLE'}
  native = copy.deepcopy(source)
  if defect == 'missing_gaze_key':
    del native['ScreenGazeDimming']
  elif defect == 'missing_mode_key':
    del native['ScreenGazeDimmingMode']
  elif defect == 'wrong_mode_default':
    native['ScreenGazeDimmingMode']['default'] = '1'
  elif defect == 'wrong_mode_type':
    native['ScreenGazeDimmingMode']['type'] = 'BOOL'
  elif defect == 'wrong_default':
    native['ScreenGazeDimPercent']['default'] = '100'
  elif defect == 'wrong_type':
    native['ScreenGazeDimPercent']['type'] = 'BOOL'
  elif defect == 'wrong_flags':
    native['ScreenGazeDimDelay']['flags'] = ['CLEAR_ON_MANAGER_START']
  stock_defects = {
    'legacy_bool_stock': ('LegacyBool', '1'),
    'legacy_string_stock': ('LegacyString', 'outdated_language'),
    'legacy_int_stock': ('LegacyInt', '17'),
    'legacy_float_stock': ('LegacyFloat', '2.5'),
    'explicit_empty_stock': ('ExplicitEmptyStock', None),
    'explicit_false_stock': ('ExplicitFalseStock', None),
    'missing_stock_and_default': ('MissingStockAndDefault', 'unexpected'),
  }
  if defect in stock_defects:
    key, value = stock_defects[defect]
    native[key]['stock'] = value
  persistent = {}
  types = SimpleNamespace(STRING=0, BOOL=1, INT=2, FLOAT=3)
  flags = SimpleNamespace(PERSISTENT=2, CLEAR_ON_MANAGER_START=4)

  class NativeParams:
    def __init__(self, _directory):
      self.values = {} if defect == 'not_persistent' else persistent

    def all_keys(self):
      return [key.encode() for key in native]

    def get_type(self, key):
      return getattr(types, native[key]['type'])

    def get_key_flag(self, key):
      return sum(getattr(flags, flag) for flag in native[key]['flags'])

    def get_default_value(self, key):
      return gates.registry_value(native[key]['default'], native[key]['type'])

    def get_stock_value(self, key):
      entry = native[key]
      effective_stock = entry['stock'] if entry['stock'] is not None else entry['default']
      return gates.registry_value(effective_stock, entry['type'])

    def get_tuning_level(self, key):
      return native[key]['tuning']

    def get_settings_tier(self, key):
      return 0

    def put(self, key, value):
      if defect == 'mode_restore_failed' and key == 'ScreenGazeDimmingMode' and value == 0:
        return
      self.values[key] = value
      if defect == 'mode_cleared_on_toggle' and key in ('ScreenManagement', 'ScreenGazeDimming'):
        self.values.pop('ScreenGazeDimmingMode', None)

    def get(self, key):
      return self.values.get(key)

  module = SimpleNamespace(Params=NativeParams, ParamKeyType=types, ParamKeyFlag=flags)
  result = gates.check_registry(module, source, tmp_path)
  assert result['verdict'] == ('PASS' if defect is None else 'FAIL')
  if defect == 'missing_gaze_key':
    assert any('ScreenGazeDimming' in error for error in result['errors'])
  if defect in ('missing_mode_key', 'wrong_mode_default', 'wrong_mode_type'):
    assert any('ScreenGazeDimmingMode' in error for error in result['errors'])
  if defect == 'mode_cleared_on_toggle':
    assert any('mode persistence failed after' in error for error in result['errors'])
  if defect == 'mode_restore_failed':
    assert 'Native Params mode default restoration failed' in result['errors']
  if defect in stock_defects:
    key, _ = stock_defects[defect]
    assert any(f'{key}: native stock=' in error for error in result['errors'])
