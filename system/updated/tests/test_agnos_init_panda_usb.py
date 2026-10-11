"""C3 internal-Panda USB rules and batched AGNOS GPIO rules in launch_chffrplus.sh agnos_init.

Adapted from official StarPilot openpilot/system/updated/tests/test_retained_agnos.py at a71528c4 (Batch AGNOS GPIO
Permission Rules; with ecff3fce C3 Panda Rules and 348b3578): only the Panda-USB/GPIO cases, run against this base's
agnos_init (AGNOS_ACCEPTED_VERSIONS check, SSH-key restore, sp_launch_timing). That branch's retained-AGNOS policy cases do
not apply here. Uses only disposable files; every effectful command is a test double, no device or launch manager runs.
"""
import os
import re
from pathlib import Path
import shlex
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
AGNOS = '19.6.20'


def shell_function(path, name):
  source = path.read_text()
  match = re.search(r'function ' + re.escape(name) + r'(?:\(\))? \{', source)
  assert match is not None
  start = match.start()
  return source[start:source.index('\n}\n', start) + 3]


class TestAgnosInitPandaUsb(unittest.TestCase):
  def setUp(self):
    self.directory = tempfile.TemporaryDirectory()
    self.addCleanup(self.directory.cleanup)
    self.root = Path(self.directory.name)
    (self.root / 'launch_env.sh').write_text(f'export AGNOS_VERSION="{AGNOS}"\nexport AGNOS_ACCEPTED_VERSIONS="{AGNOS}"\n')

  def run_shell(self, function, tail, current=AGNOS, model='comma tizi', udev_failure=''):
    version = self.root / 'VERSION'
    version.write_text(current)
    device_model = self.root / 'model'
    device_model.write_text(model + '\0')
    udevadm = self.root / 'udevadm'
    udevadm.write_text('''#!/bin/sh
echo "$*" >> "$UDEV_TEST_LOG"
case "$*" in "trigger --settle "*) exit 1;; esac
if [ "$UDEV_TEST_FAILURE" = hang ]; then sleep 60; fi
if [ "$UDEV_TEST_FAILURE" = "$1" ]; then exit 1; fi
''')
    udevadm.chmod(0o755)
    gpio_rules = self.root / 'stock-gpio.rules'
    if not gpio_rules.exists():
      gpio_rules.write_text('SUBSYSTEM=="gpio*", PROGRAM="/bin/sh -c \'find -L /sys/class/gpio/ -maxdepth 2 ' +
                            '-exec chown root:gpio {} \\; -exec chmod 770 {} \\; || true\'"\n' +
                            'SUBSYSTEM=="gpio", KERNEL=="gpiochip[0]", GROUP="gpio", MODE="660"\n')
    for name, body in {
      'mountpoint': '[ -f "$UDEV_TEST_MOUNTED" ]',
      'mount': '''[ "$UDEV_TEST_FAILURE" != mount ] || exit 1
echo "$*" >> "$UDEV_TEST_MOUNT_LOG"
cp "$2" "$3"
touch "$UDEV_TEST_MOUNTED"''',
    }.items():
      command = self.root / name
      command.write_text('#!/bin/sh\n' + body + '\n')
      command.chmod(0o755)
    script = self.root / 'test.sh'
    # Replace only device observations in a copy of the actual function.
    function = function.replace('/VERSION', str(version))
    function = function.replace('/cache/reset_backup', str(self.root / 'no-reset-backup'))
    function = function.replace('/sys/firmware/devicetree/base/model', str(device_model))
    function = function.replace('/run/udev/rules.d', str(self.root / 'udev-rules'))
    function = function.replace('/etc/udev/rules.d/99-gpio.rules', str(gpio_rules))
    function = function.replace('timeout -k 1s 5s', 'timeout -k 0.1s 0.2s')
    script.write_text('''source "$1/launch_env.sh"
DIR="$1"
log="$1/effects"
sp_launch_timing() { :; }
sudo() { if [ "$1" = timeout ]; then "$@"; else echo sudo >> "$log"; fi; }
''' + function + '\n' + tail + '\n')
    environment = dict(os.environ, PATH=f'{self.root}:{os.environ["PATH"]}',
                       UDEV_TEST_LOG=str(self.root / 'udev.log'), UDEV_TEST_FAILURE=udev_failure,
                       UDEV_TEST_MOUNTED=str(self.root / 'mounted'), UDEV_TEST_MOUNT_LOG=str(self.root / 'mount.log'))
    return subprocess.run(['bash', str(script), str(self.root)], env=environment,
                          capture_output=True, text=True, timeout=5)

  def test_panda_usb_permissions_are_tici_only_and_repeatable(self):
    function = shell_function(ROOT / 'launch_chffrplus.sh', 'agnos_init')
    for model in ('comma tizi', 'comma mici', 'comma tici2', 'unknown', 'comma tici'):
      with self.subTest(model=model):
        result = self.run_shell(function, 'agnos_init', model=model)
        self.assertEqual(result.returncode, 0, result.stderr)
        if model != 'comma tici':
          self.assertFalse((self.root / 'udev-rules').exists())
          self.assertFalse((self.root / 'udev.log').exists())
          self.assertFalse((self.root / 'mount.log').exists())
    rules = self.root / 'udev-rules/99-starpilot-panda.rules'
    expected = ('SUBSYSTEM=="usb", ATTRS{idVendor}=="3801", ATTRS{idProduct}=="ddcc", MODE="0666"\n' +
                'SUBSYSTEM=="usb", ATTRS{idVendor}=="3801", ATTRS{idProduct}=="ddee", MODE="0666"\n')
    self.assertEqual(rules.read_text(), expected)
    gpio_rules = self.root / 'stock-gpio.rules'
    self.assertEqual(gpio_rules.read_text().count('{} +'), 2)
    self.assertNotIn('\\;', gpio_rules.read_text())
    self.assertIn('KERNEL=="gpiochip[0]", GROUP="gpio", MODE="660"', gpio_rules.read_text())
    self.assertEqual(len((self.root / 'mount.log').read_text().splitlines()), 1)
    commands = ['control --reload-rules', *[
      f'trigger --subsystem-match=usb --attr-match=idVendor=3801 --attr-match=idProduct={product}'
      for product in ('ddcc', 'ddee')]]
    self.assertEqual((self.root / 'udev.log').read_text().splitlines(), commands)
    result = self.run_shell(function, 'agnos_init', model='comma tici')
    self.assertEqual(result.returncode, 0, result.stderr)
    self.assertEqual(rules.read_text(), expected)
    self.assertEqual((self.root / 'udev.log').read_text().splitlines(), commands * 2)
    self.assertEqual(len((self.root / 'mount.log').read_text().splitlines()), 1)

  def test_panda_usb_permission_failure_never_blocks_manager_start(self):
    function = shell_function(ROOT / 'launch_chffrplus.sh', 'agnos_init')
    for failure in ('mount', 'control', 'trigger', 'hang'):
      with self.subTest(failure=failure):
        result = self.run_shell(function, 'agnos_init && echo manager-start', model='comma tici', udev_failure=failure)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('Panda USB permission setup failed; continuing startup.', result.stdout)
        self.assertIn('manager-start', result.stdout)
    rules_directory = self.root / 'udev-rules'
    for child in rules_directory.iterdir():
      child.unlink()
    rules_directory.rmdir()
    rules_directory.write_text('not a directory')
    result = self.run_shell(function, 'agnos_init && echo manager-start', model='comma tici')
    self.assertEqual(result.returncode, 0, result.stderr)
    self.assertIn('Panda USB permission setup failed; continuing startup.', result.stdout)
    self.assertIn('manager-start', result.stdout)

  def test_gpio_permissions_batch_commands_without_changing_targets(self):
    function = shell_function(ROOT / 'launch_chffrplus.sh', 'agnos_init')
    self.run_shell(function, 'agnos_init', model='comma tici')
    rule = (self.root / 'stock-gpio.rules').read_text().splitlines()[0]
    command = shlex.split(re.search(r'PROGRAM="(.*)"', rule)[1])[2]
    self.assertEqual(command.count('{} +'), 2)

    gpio = self.root / 'gpio'
    gpio.mkdir()
    pin = self.root / 'pin'
    pin.mkdir()
    for name in ('direction', 'value', 'edge', 'uevent'):
      (pin / name).touch()
    (pin / 'subsystem').symlink_to(gpio, target_is_directory=True)
    (gpio / 'gpio134').symlink_to(pin, target_is_directory=True)
    (gpio / 'export').touch()
    command = command.replace('/sys/class/gpio/', str(gpio) + '/').replace('root:gpio', f'{os.getuid()}:{os.getgid()}')
    commands = self.root / 'permission-commands'
    commands.mkdir()
    log = self.root / 'permission.log'
    for name in ('chown', 'chmod'):
      wrapper = commands / name
      wrapper.write_text(f'#!/bin/sh\necho {name} >> "$GPIO_TEST_LOG"\nexec {shlex.quote(shutil.which(name))} "$@"\n')
      wrapper.chmod(0o755)
    environment = dict(os.environ, PATH=f'{commands}:{os.environ["PATH"]}', GPIO_TEST_LOG=str(log))
    paths = [gpio, gpio / 'export', pin, *(pin / name for name in ('direction', 'value', 'edge', 'uevent'))]
    results = []
    for program in (command.replace('{} +', r'{} \;'), command):
      for path in paths:
        path.chmod(0o755 if path.is_dir() else 0o644)
      log.write_text('')
      result = subprocess.run(['sh', '-c', program], env=environment, capture_output=True, text=True, timeout=5)
      self.assertEqual(result.returncode, 0, result.stderr)
      results.append([(path.stat().st_mode, path.stat().st_uid, path.stat().st_gid) for path in paths])
      if program == command:
        self.assertEqual(log.read_text().splitlines(), ['chown', 'chmod'])
      else:
        self.assertGreater(len(log.read_text().splitlines()), 2)
    self.assertEqual(results[0], results[1])
    self.assertTrue(all(path.stat().st_mode & 0o777 == 0o770 for path in paths))

  def test_agnos_mismatch_still_takes_the_update_path_first(self):
    # The added block sits after this base's unchanged AGNOS check: with a non-accepted /VERSION the existing
    # update path runs (its commands are doubled/absent here) and only then the tici block.
    function = shell_function(ROOT / 'launch_chffrplus.sh', 'agnos_init')
    self.assertLess(function.index('AGNOS_UPDATE_REQUIRED'), function.index('99-starpilot-panda.rules'))
    self.assertLess(function.index('99-starpilot-panda.rules'), function.index('sp_launch_timing "agnos_init_done"'))


if __name__ == '__main__':
  unittest.main()
