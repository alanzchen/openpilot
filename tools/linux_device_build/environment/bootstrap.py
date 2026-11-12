#!/usr/bin/env python3
"""Add locked build/test tools without replacing AGNOS-managed native packages."""
import hashlib
import json
from pathlib import Path
import platform
import subprocess
import sys
import tempfile
import tomllib
import urllib.request

ROOT = Path('/opt/linux-device-build')
SITE = Path('/usr/local/venv/lib/python3.12/site-packages')
# These are tools and missing generator/test dependencies, not a replacement
# device runtime. Their exact versions and wheel hashes come from repo uv.lock.
TOOLS = (
  'scons', 'cython', 'casadi', 'scipy', 'pyyaml', 'hypothesis', 'sortedcontainers',
  'parameterized', 'pytest', 'pytest-asyncio', 'pytest-cpp', 'pytest-mock', 'pytest-randomly',
  'pytest-repeat', 'pytest-subtests', 'pytest-timeout',
  'iniconfig', 'pluggy', 'execnet',
)
PROTECTED = ('capnproto', 'ffmpeg', 'eigen', 'raylib', 'gcc_arm_none_eabi')


def protected_bytes():
  files = {}
  for name in PROTECTED:
    package = SITE / name
    if not package.is_dir():
      raise RuntimeError(f'Missing device-managed dependency {package}')
    for path in sorted(package.rglob('*')):
      if '__pycache__' not in path.parts and path.is_file():
        with path.open('rb') as source:
          files[str(path.relative_to(SITE))] = hashlib.file_digest(source, 'sha256').hexdigest()
  return files


def wheel_for(package):
  from packaging.tags import sys_tags
  from packaging.utils import parse_wheel_filename
  # Packaging evaluates the actual pinned Python/ARM64/glibc tag set. Prefer
  # native CPython wheels over pure-Python fallbacks when both are locked.
  supported = {tag: rank for rank, tag in enumerate(sys_tags())}
  candidates = []
  for wheel in package.get('wheels', []):
    name = wheel['url'].rsplit('/', 1)[-1]
    tags = parse_wheel_filename(name)[3]
    ranks = [supported[tag] for tag in tags if tag in supported]
    if ranks:
      candidates.append((min(ranks), name, wheel))
  if not candidates:
    raise RuntimeError(f'No locked AGNOS-compatible wheel for {package["name"]}')
  return sorted(candidates)[0][2]


def main():
  pins = json.loads((ROOT / 'pins.json').read_text())
  if platform.machine() != 'aarch64' or platform.python_version() != pins['python']:
    raise RuntimeError('Builder must execute natively inside pinned AGNOS ARM64/Python userspace')
  if Path('/VERSION').read_text().strip() != pins['version']:
    raise RuntimeError('AGNOS version differs from build pin')
  before = protected_bytes()
  packages = {p['name']: p for p in tomllib.loads((ROOT / 'uv.lock').read_text())['package']}
  overlay = ROOT / 'packages'
  overlay.mkdir(exist_ok=True)
  installed = {}
  with tempfile.TemporaryDirectory(prefix='locked-build-tools-') as temp:
    paths = []
    for name in TOOLS:
      package = packages[name]
      source = package['source']
      if 'git' in source:
        url, revision = source['git'].split('#')
        url = url.split('?')[0]
        paths.append(f'{name} @ git+{url}@{revision}')
      else:
        wheel = wheel_for(package)
        path = Path(temp) / wheel['url'].rsplit('/', 1)[-1]
        print(f'Downloading locked {name}=={package["version"]}', flush=True)
        urllib.request.urlretrieve(wheel['url'], path)
        with path.open('rb') as stream:
          actual = hashlib.file_digest(stream, 'sha256').hexdigest()
        if actual != wheel['hash'].removeprefix('sha256:'):
          raise RuntimeError(f'Locked wheel hash mismatch: {name}')
        paths.append(str(path))
      installed[name] = package['version']
    # --target cannot uninstall or overwrite any AGNOS venv files. Keep Python
    # sys.prefix=/usr/local/venv for correct on-device ffmpeg runtime RPATHs.
    subprocess.run([sys.executable, '-m', 'pip', 'install', '--no-cache-dir', '--no-deps',
                    '--no-build-isolation', '--target', str(overlay), *paths], check=True)
  if before != protected_bytes():
    raise RuntimeError('Device-managed dependency bytes changed while installing build tools')
  (ROOT / 'protected-packages.json').write_text(json.dumps(before, sort_keys=True, indent=2) + '\n')
  (ROOT / 'build-tools.json').write_text(json.dumps(installed, sort_keys=True, indent=2) + '\n')
  # Check native code generators and the managed library search paths now,
  # before an expensive project build starts.
  subprocess.run(['clang', '--version'], check=True)
  subprocess.run(['arm-none-eabi-gcc', '--version'], check=True)
  check = 'import capnproto, ffmpeg, eigen, numpy, Cython, SCons, pytest; ' + 'print("AGNOS managed native dependencies and locked build tools imported")'
  subprocess.run([sys.executable, '-c', check], check=True)


if __name__ == '__main__':
  main()
