#!/usr/bin/env python3
"""Make a Docker context from the hash-pinned AGNOS 19.6.20 system image.

Runs on the host, requiring e2fsprogs (debugfs) and libsparse-utils (simg2img).
No mounts, root privileges, or existing device/build artifacts are required.
"""
import argparse
import hashlib
import json
import lzma
from pathlib import Path
import shutil
import subprocess
import tempfile
import urllib.error
import urllib.request

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[2]


def digest(path):
  with path.open('rb') as source:
    return hashlib.file_digest(source, 'sha256').hexdigest()


def verify(path, expected):
  actual = digest(path)
  if actual != expected:
    raise RuntimeError(f'{path}: SHA-256 {actual}, expected {expected}')


def download(urls, destination):
  partial = destination.with_suffix(destination.suffix + '.part')
  for url in urls:
    try:
      print(f'Downloading {url}', flush=True)
      with urllib.request.urlopen(url, timeout=120) as source, partial.open('wb') as target:
        shutil.copyfileobj(source, target, 8 * 1024 * 1024)
      partial.replace(destination)
      return
    except (OSError, urllib.error.URLError) as error:
      partial.unlink(missing_ok=True)
      print(f'Download failed: {error}', flush=True)
  raise RuntimeError('All pinned AGNOS download URLs failed')


def prepare(cache, output):
  pins = json.loads((HERE / 'pins.json').read_text())
  # Catch an accidental pin/manifest mismatch instead of quietly building a
  # different device ABI when one of these inputs changes independently.
  manifest = json.loads((REPO / 'system/hardware/tici/agnos.json').read_text())
  partition = next(p for p in manifest if p['name'] == 'system')
  stamp = json.loads((REPO / 'build_stamps/mac-larch64.json').read_text())['agnos']
  if not (partition['hash'] == pins['system_sha256'] == stamp['AGNOS_SYSTEM_HASH'] and
          partition['hash_raw'] == pins['raw_sha256'] == stamp['AGNOS_SYSTEM_HASH_RAW'] and
          partition['size'] == pins['raw_size'] and pins['version'] == stamp['AGNOS_VERSION']):
    raise RuntimeError('AGNOS environment pins differ from device manifest/build stamp')
  for name in ('debugfs', 'simg2img', 'tar'):
    if shutil.which(name) is None:
      raise RuntimeError(f'Missing host tool {name}; install e2fsprogs android-sdk-libsparse-utils')
  cache.mkdir(parents=True, exist_ok=True)
  output.mkdir(parents=True, exist_ok=True)
  compressed = cache / 'system29.img.xz'
  sparse = cache / 'system29.sparse.img'
  raw = cache / 'system29.ext4.img'
  if not sparse.exists():
    if not compressed.exists():
      download(pins['urls'], compressed)
    partial = sparse.with_suffix('.part')
    print('Decompressing pinned AGNOS system image', flush=True)
    with lzma.open(compressed) as source, partial.open('wb') as target:
      shutil.copyfileobj(source, target, 8 * 1024 * 1024)
    partial.replace(sparse)
  # Always verify caches; a cache hit is not evidence of image integrity.
  verify(sparse, pins['system_sha256'])
  if not raw.exists():
    partial = raw.with_suffix('.part')
    subprocess.run(['simg2img', str(sparse), str(partial)], check=True)
    partial.replace(raw)
  if raw.stat().st_size != pins['raw_size']:
    raise RuntimeError('Expanded AGNOS partition size differs from pin')
  verify(raw, pins['raw_sha256'])
  with tempfile.TemporaryDirectory(prefix='agnos-root-', dir=cache) as directory:
    root = Path(directory)
    print('Extracting verified ext4 userspace with debugfs', flush=True)
    result = subprocess.run(['debugfs', '-R', f'rdump / {root}', str(raw)], text=True, capture_output=True, check=True)
    (output / 'debugfs.log').write_text(result.stdout + result.stderr)
    # debugfs returns success for some rdump errors. Unprivileged ownership
    # restoration is harmless; reject every other diagnostic rather than
    # accepting a partial runtime. tar normalizes container owners below.
    errors = [line for line in result.stderr.splitlines()
              if line and not line.startswith('debugfs ') and 'while changing ownership of ' not in line]
    if errors:
      raise RuntimeError('AGNOS extraction failed:\n' + '\n'.join(errors[:20]))
    if (root / 'VERSION').read_text().strip() != pins['version']:
      raise RuntimeError('Extracted AGNOS /VERSION differs from pin')
    # Device transient/mount locations must not enter the container archive.
    for name in ('dev', 'proc', 'sys', 'run', 'tmp', 'lost+found'):
      path = root / name
      if path.is_dir() and not path.is_symlink():
        shutil.rmtree(path)
        path.mkdir(mode=0o1777 if name == 'tmp' else 0o755)
    archive = output / 'agnos-rootfs.tar'
    subprocess.run(['tar', '--sort=name', '--numeric-owner', '--owner=0', '--group=0',
                    '-C', str(root), '-cf', str(archive), '.'], check=True)
  provenance = {**pins, 'rootfs_tar_sha256': digest(archive), 'uv_lock_sha256': digest(REPO / 'uv.lock')}
  (output / 'provenance.json').write_text(json.dumps(provenance, indent=2) + '\n')
  for name in ('Dockerfile', 'pins.json', 'bootstrap.py', 'sitecustomize.py'):
    shutil.copyfile(HERE / name, output / name)
  shutil.copyfile(REPO / 'uv.lock', output / 'uv.lock')
  print(f'Prepared verified Docker context at {output}', flush=True)


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument('--cache-dir', type=Path, required=True)
  parser.add_argument('--output-dir', type=Path, required=True)
  args = parser.parse_args()
  prepare(args.cache_dir.resolve(), args.output_dir.resolve())


if __name__ == '__main__':
  main()
