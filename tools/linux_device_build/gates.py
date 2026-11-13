#!/usr/bin/env python3
"""Fail-closed checks for independently built AGNOS device artifacts.

Run ABI and Params gates inside the pinned AGNOS root, not an Ubuntu host.
readelf inspects dependencies without executing candidate ELF files. This checks
link/load compatibility; camera, GPU and vehicle behavior still need hardware.
"""
from __future__ import annotations

import argparse
import datetime
import hashlib
import importlib
import importlib.machinery
import json
import os
import platform
import posixpath
import re
import stat
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

BASELINE = "build_stamps/mac-larch64.json"
DEFAULT_LIBRARY_DIRS = (
  "/usr/local/lib", "/lib/aarch64-linux-gnu", "/usr/lib/aarch64-linux-gnu",
  "/lib", "/usr/lib", "/system/vendor/lib64", "/vendor/lib64",
)
GATE_LIMITATIONS = ["Qualcomm GPU/OpenCL runtime", "camera/modeld on hardware", "panda flashing", "vehicle behavior"]


class GateError(RuntimeError):
  pass


def sha256(path: Path) -> str:
  with path.open("rb") as stream:
    return hashlib.file_digest(stream, "sha256").hexdigest()


def artifact_paths(root: Path, artifacts_file: Path | None = None) -> list[str]:
  if artifacts_file:
    paths = [line.strip() for line in artifacts_file.read_text().splitlines() if line.strip() and not line.startswith("#")]
  else:
    paths = list(json.loads((root / BASELINE).read_text())["package_inventory"])
  # This deployment marker must only be installed after every gate passes.
  paths = [path for path in paths if path != "prebuilt"]
  if not paths or len(set(paths)) != len(paths):
    raise GateError("Artifact inventory is empty or contains duplicates")
  for name in paths:
    path = PurePosixPath(name)
    if path.is_absolute() or ".." in path.parts or str(path) != name:
      raise GateError(f"Unsafe artifact path: {name}")
  return sorted(paths)


def artifact_manifest(root: Path, paths: list[str], source_sha: str) -> dict:
  if not re.fullmatch(r"[0-9a-f]{40}", source_sha):
    raise GateError("A full source commit SHA is required")
  artifacts = {}
  for name in paths:
    path = root / name
    if path.is_symlink() or not path.is_file():
      raise GateError(f"Artifact is missing or is not a regular file: {name}")
    if not path.resolve().is_relative_to(root.resolve()):
      raise GateError(f"Artifact escapes checkout: {name}")
    mode = stat.S_IMODE(path.stat().st_mode)
    if mode not in (0o644, 0o755):
      raise GateError(f"Unexpected artifact permissions for {name}: {mode:o}")
    artifacts[name] = {"sha256": sha256(path), "mode": f"{stat.S_IFREG | mode:o}"}
  return {"schema": 1, "verdict": "PASS", "source_sha": source_sha, "artifacts": artifacts}


def compare_manifests(first: dict, second: dict) -> dict:
  if any(manifest.get("schema") != 1 or manifest.get("verdict") != "PASS" or not manifest.get("artifacts")
         for manifest in (first, second)):
    raise GateError("Only successful, nonempty artifact manifests can be compared")
  first_files, second_files = first["artifacts"], second["artifacts"]
  changed = sorted(name for name in first_files.keys() & second_files.keys() if first_files[name] != second_files[name])
  added, removed = sorted(second_files.keys() - first_files.keys()), sorted(first_files.keys() - second_files.keys())
  same_source = first.get("source_sha") == second.get("source_sha") and bool(re.fullmatch(r"[0-9a-f]{40}", first.get("source_sha", "")))
  return {"schema": 1, "verdict": "PASS" if same_source and not (changed or added or removed) else "FAIL",
          "source_sha": first.get("source_sha"), "same_source": same_source, "compared_artifacts": len(first_files),
          "changed": changed, "added": added, "removed": removed, "normalizations": []}


@dataclass
class ElfInfo:
  bits: int
  machine: int
  elf_type: int
  needed: list[str]
  rpath: list[str]
  runpath: list[str]
  interpreter: str | None
  required_versions: dict[str, set[str]]
  defined_versions: set[str]


def parse_elf(path: Path, readelf: str = "readelf") -> ElfInfo | None:
  with path.open("rb") as stream:
    header = stream.read(20)
  if header[:4] != b"\x7fELF":
    return None
  if len(header) != 20 or header[4] not in (1, 2) or header[5] != 1:
    raise GateError(f"Invalid or non-little-endian ELF: {path}")
  elf_type, machine = struct.unpack_from("<HH", header, 16)
  result = subprocess.run([readelf, "--wide", "--program-headers", "--dynamic", "--version-info", str(path)],
                          check=True, capture_output=True, text=True, env={**os.environ, "LC_ALL": "C"})
  return parse_elf_output(result.stdout, bits=32 if header[4] == 1 else 64, machine=machine, elf_type=elf_type)


def parse_elf_output(output: str, *, bits=64, machine=183, elf_type=3) -> ElfInfo:
  tags = {}
  for tag, value in re.findall(r"\((NEEDED|RPATH|RUNPATH)\)[^\n]*\[([^\]]+)\]", output):
    tags.setdefault(tag, []).append(value)
  interpreter = re.search(r"Requesting program interpreter:\s*([^\]\s]+)", output)
  required: dict[str, set[str]] = {}
  definitions = set()
  section = library = ""
  for line in output.splitlines():
    if "Version needs section" in line:
      section = "needs"
    elif "Version definition section" in line:
      section = "definitions"
    elif "Version symbols section" in line:
      section = ""
    if section == "needs":
      file_match = re.search(r"\bFile:\s*(\S+)", line)
      if file_match:
        library = file_match[1]
      version = re.search(r"\bName:\s*(\S+)", line)
      if version and library:
        required.setdefault(library, set()).add(version[1])
    elif section == "definitions":
      version = re.search(r"\bName:\s*(\S+)", line)
      if version:
        definitions.add(version[1])
  return ElfInfo(bits, machine, elf_type, tags.get("NEEDED", []),
                 [part for entry in tags.get("RPATH", []) for part in entry.split(":")],
                 [part for entry in tags.get("RUNPATH", []) for part in entry.split(":")],
                 interpreter[1] if interpreter else None, required, definitions)


def in_sysroot(sysroot: Path, runtime_path: str) -> Path:
  """Resolve absolute symlinks using the target root rather than the host root."""
  runtime = posixpath.normpath(runtime_path)
  if not runtime.startswith("/"):
    raise GateError(f"Library path is not absolute: {runtime_path}")
  for _ in range(40):
    parts = PurePosixPath(runtime).parts[1:]
    for index in range(len(parts)):
      candidate = sysroot.joinpath(*parts[:index + 1])
      if candidate.is_symlink():
        target = os.readlink(candidate)
        prefix = "/" + "/".join(parts[:index])
        runtime = posixpath.normpath(posixpath.join(target if target.startswith("/") else posixpath.join(prefix, target), *parts[index + 1:]))
        break
    else:
      return sysroot.joinpath(*parts)
  raise GateError(f"Too many symlinks resolving {runtime_path}")


def library_directories(sysroot: Path) -> list[str]:
  directories = list(DEFAULT_LIBRARY_DIRS)
  seen = set()

  def read_config(path: Path):
    if not path.is_file() or path in seen:
      return
    seen.add(path)
    for line in path.read_text().splitlines():
      line = line.partition("#")[0].strip()
      if line.startswith("include "):
        pattern = line.split(None, 1)[1]
        for included in sorted(sysroot.glob(pattern.lstrip("/"))):
          read_config(included)
      elif line.startswith("/"):
        directories.append(line)

  read_config(sysroot / "etc/ld.so.conf")
  return list(dict.fromkeys(directories))


def abi_gate(root: Path, paths: list[str], sysroot: Path, *, inspect=parse_elf) -> dict:
  root, sysroot = root.resolve(), sysroot.resolve()
  errors, records = [], {}
  details = {}

  def info(path):
    if path not in details:
      details[path] = inspect(path)
    return details[path]

  def check_arch(path, elf, firmware=False):
    expected = (32, 40) if firmware else (64, 183)
    if (elf.bits, elf.machine) != expected:
      errors.append(f"{path}: expected ELF{expected[0]} machine {expected[1]}, got ELF{elf.bits} machine {elf.machine}")

  def candidate(runtime):
    path = Path(posixpath.normpath(runtime))
    return path.resolve() if path.is_relative_to(root) else in_sysroot(sysroot, str(path))

  def expand(entries, owner):
    expanded = []
    for entry in entries:
      if not entry or (not entry.startswith("/") and not entry.startswith(("$ORIGIN", "${ORIGIN}"))):
        errors.append(f"{owner}: relative/empty loader search path {entry!r}")
        continue
      if "$" not in entry and (entry == str(root) or entry.startswith((str(root) + "/", "/work/", "/workspace/", "/tmp/", "/home/runner/", "/build/"))):
        errors.append(f"{owner}: build-host loader search path {entry}")
        continue
      origin = "/" + owner.parent.relative_to(sysroot).as_posix() if sysroot != Path("/") and owner.is_relative_to(sysroot) else str(owner.parent)
      value = entry.replace("${ORIGIN}", origin).replace("$ORIGIN", origin)
      if "$" in value:
        errors.append(f"{owner}: unsupported loader token in {entry}")
        continue
      expanded.append(posixpath.normpath(value))
    return expanded

  queue = []
  firmware_count = 0
  for name in paths:
    path = root / name
    if not path.is_file():
      errors.append(f"Missing artifact: {name}")
      continue
    elf = info(path)
    if elf is None:
      if path.suffix in (".so", ".os", ".elf") or (os.access(path, os.X_OK) and not name.startswith("panda/board/")):
        errors.append(f"Expected ELF artifact: {name}")
      continue
    firmware = name.startswith("panda/board/")
    check_arch(path, elf, firmware)
    if firmware:
      firmware_count += 1
      if elf.interpreter or elf.needed:
        errors.append(f"Firmware unexpectedly has dynamic dependencies: {name}")
    else:
      queue.append((path, []))

  defaults = library_directories(sysroot)
  initial_native = len(queue)
  if initial_native == 0:
    errors.append("No device ELF outputs were checked")
  while queue:
    path, inherited = queue.pop(0)
    if str(path) in records:
      continue
    elf = info(path)
    if elf is None:
      errors.append(f"Dependency is not ELF: {path}")
      continue
    check_arch(path, elf)
    own_rpath = expand(elf.rpath, path) if not elf.runpath else []
    runpath = expand(elf.runpath, path)
    search = own_rpath + inherited + runpath + defaults
    resolved = {}
    record = {"bits": elf.bits, "machine": elf.machine, "needed": resolved,
              "rpath": elf.rpath, "runpath": elf.runpath, "interpreter": elf.interpreter}
    records[str(path)] = record
    if elf.interpreter:
      if elf.interpreter != "/lib/ld-linux-aarch64.so.1":
        errors.append(f"Unexpected device interpreter in {path}: {elf.interpreter}")
      interpreter_path = candidate(elf.interpreter)
      if not interpreter_path.is_file():
        errors.append(f"Missing device interpreter: {elf.interpreter}")
      else:
        queue.append((interpreter_path, []))
    for library in elf.needed:
      if "/" in library:
        errors.append(f"Non-SONAME dependency in {path}: {library}")
        continue
      provider = next((located for directory in search if (located := candidate(posixpath.join(directory, library))).is_file()), None)
      if provider is None:
        errors.append(f"Unresolved device dependency: {path} -> {library}")
        continue
      resolved[library] = str(provider)
      provided = info(provider)
      if provided is None:
        errors.append(f"Dependency is not ELF: {provider}")
        continue
      missing_versions = elf.required_versions.get(library, set()) - provided.defined_versions
      if missing_versions:
        errors.append(f"Unavailable ABI versions: {path} -> {library}: {', '.join(sorted(missing_versions))}")
      queue.append((provider, own_rpath + inherited))
  return {"schema": 1, "verdict": "FAIL" if errors else "PASS", "device_outputs": initial_native,
          "firmware_outputs": firmware_count, "elf_closure": records, "errors": errors,
          "loader_environment": "LD_LIBRARY_PATH is deliberately ignored", "not_covered": GATE_LIMITATIONS}


def registry_source(path: Path) -> dict[str, dict]:
  registry = {}
  source = path.read_text()
  declared = re.findall(r'^\s*\{"([^"\n]+)",', source, re.MULTILINE)
  for key, attributes in re.findall(r'^\s*\{"([^"\n]+)",\s*\{(.*?)\}\},?\s*(?://[^\n]*)?$', source, re.MULTILINE):
    tokens, current = [], []
    quoted = escaped = False
    for character in attributes:
      if character == ',' and not quoted:
        tokens.append(''.join(current).strip())
        current = []
        continue
      current.append(character)
      if escaped:
        escaped = False
      elif character == '\\' and quoted:
        escaped = True
      elif character == '"':
        quoted = not quoted
    tokens.append(''.join(current).strip())
    if key in registry or len(tokens) < 2:
      raise GateError(f"Invalid or duplicated source registry entry: {key}")
    def optional(index, fields=tokens):
      if len(fields) <= index or fields[index] == "std::nullopt":
        return None
      if fields[index].startswith('"'):
        return json.loads(fields[index])
      # One default is generated from a Cap'n Proto enum. Resolve the source
      # ordinal rather than copying a potentially stale native registry value.
      expression = re.fullmatch(r'std::to_string\(static_cast<int>\(cereal::(\w+)::(\w+)\)\)', fields[index])
      if expression:
        schema = (path.parent.parent / "cereal/log.capnp").read_text()
        enum = re.search(r'enum\s+' + re.escape(expression[1]) + r'\s*\{([^}]+)\}', schema)
        member = re.search(r'\b' + re.escape(expression[2].lower()) + r'\s*@(\d+)', enum[1]) if enum else None
        if member:
          return member[1]
      raise GateError(f"Unsupported source registry expression: {fields[index]}")

    registry[key] = {"flags": tokens[0].split("|"), "type": tokens[1], "default": optional(2), "stock": optional(3),
                     "tuning": int(tokens[4]) if len(tokens) > 4 else 0,
                     "tier": tokens[5] if len(tokens) > 5 else "SETTINGS_ADVANCED"}
  if not registry:
    raise GateError("Source Params registry is empty")
  if set(declared) != registry.keys() or len(declared) != len(registry):
    raise GateError(f"Unparsed or duplicate source Params declarations: {sorted(set(declared) - registry.keys())}")
  return registry


def registry_value(raw, kind):
  if raw is None:
    return None
  converters = {"BOOL": lambda value: value == "1", "INT": lambda value: int(float(value)), "FLOAT": float,
                "STRING": str, "BYTES": str.encode, "JSON": json.loads, "TIME": datetime.datetime.fromisoformat}
  return converters[kind](raw)


def check_registry(module, expected: dict, directory: Path) -> dict:
  errors = []
  params = module.Params(str(directory))
  actual_keys = {key.decode() if isinstance(key, bytes) else key for key in params.all_keys()}
  if actual_keys != expected.keys():
    errors.append(f"Registry names differ: missing={sorted(expected.keys() - actual_keys)}, extra={sorted(actual_keys - expected.keys())}")
  for key in sorted(expected.keys() & actual_keys):
    entry = expected[key]
    flags = sum(int(getattr(module.ParamKeyFlag, flag.strip())) for flag in entry["flags"])
    # getStockValue() falls back only when the optional stock field is absent.
    # An explicitly empty stock string is present and must stay empty.
    effective_stock = entry["stock"] if entry["stock"] is not None else entry["default"]
    checks = {
      "type": (int(params.get_type(key)), int(getattr(module.ParamKeyType, entry["type"]))),
      "flags": (int(params.get_key_flag(key)), flags),
      "default": (params.get_default_value(key), registry_value(entry["default"], entry["type"])),
      "stock": (params.get_stock_value(key), registry_value(effective_stock, entry["type"])),
      "tuning": (params.get_tuning_level(key), entry["tuning"]),
      "tier": (params.get_settings_tier(key), 0 if entry["tier"] == "SETTINGS_SIMPLE" else 1),
    }
    for attribute, (actual, wanted) in checks.items():
      if actual != wanted:
        errors.append(f"{key}: native {attribute}={actual!r}, source={wanted!r}")
  roundtrip = {"ScreenManagement": True, "ScreenGazeDimming": True, "ScreenGazeDimmingMode": 1, "ScreenGazeDimPercent": 35,
               "ScreenGazeDimDelay": 8, "ScreenGazeScreenSide": 1, "ScreenGazeYawThreshold": 25}
  for key, value in roundtrip.items():
    if key in actual_keys:
      params.put(key, value)
    else:
      errors.append(f"Native Params does not register required screen key: {key}")
  reopened = module.Params(str(directory))
  for key, value in roundtrip.items():
    if key in actual_keys and reopened.get(key) != value:
      errors.append(f"Native Params persistence failed: {key}")
  if "ScreenGazeDimmingMode" in actual_keys:
    for toggle in ("ScreenManagement", "ScreenGazeDimming"):
      if toggle not in actual_keys:
        continue
      for enabled in (False, True):
        params.put(toggle, enabled)
        if module.Params(str(directory)).get("ScreenGazeDimmingMode") != 1:
          errors.append(f"Native Params mode persistence failed after {toggle}={enabled}")
    params.put("ScreenGazeDimmingMode", 0)
    if module.Params(str(directory)).get("ScreenGazeDimmingMode") != 0:
      errors.append("Native Params mode default restoration failed")
  return {"schema": 1, "verdict": "FAIL" if errors else "PASS", "keys_source": len(expected),
          "keys_native": len(actual_keys), "roundtrip_keys": sorted(roundtrip), "errors": errors}


def params_gate(root: Path) -> dict:
  if platform.machine() not in ("aarch64", "arm64") or sys.version_info[:2] != (3, 12):
    raise GateError("Params gate requires native ARM64 AGNOS Python 3.12")
  sys.path.insert(0, str(root))
  module = importlib.import_module("openpilot.common.params_pyx")
  path = Path(module.__file__).resolve()
  if path != (root / "common/params_pyx.so").resolve() or not isinstance(module.__loader__, importlib.machinery.ExtensionFileLoader):
    raise GateError(f"Params gate did not load the candidate native extension: {path}")
  with tempfile.TemporaryDirectory(prefix="linux-device-params-") as directory:
    result = check_registry(module, registry_source(root / "common/params_keys.h"), Path(directory))
  result.update(extension_sha256=sha256(path), registry_sha256=sha256(root / "common/params_keys.h"))
  return result


def main(argv=None) -> int:
  parser = argparse.ArgumentParser(description=__doc__)
  commands = parser.add_subparsers(dest="command", required=True)
  for command in ("abi", "params", "manifest"):
    subparser = commands.add_parser(command)
    subparser.add_argument("--root", type=Path, required=True)
    subparser.add_argument("--output", type=Path, required=True)
    if command != "params":
      subparser.add_argument("--artifacts-file", type=Path)
    if command == "abi":
      subparser.add_argument("--sysroot", type=Path, default=Path("/"))
  compare = commands.add_parser("compare")
  compare.add_argument("--first", type=Path, required=True)
  compare.add_argument("--second", type=Path, required=True)
  compare.add_argument("--output", type=Path, required=True)
  args = parser.parse_args(argv)
  try:
    if args.command == "compare":
      result = compare_manifests(json.loads(args.first.read_text()), json.loads(args.second.read_text()))
    else:
      root = args.root.resolve()
      if args.command == "params":
        result = params_gate(root)
      else:
        paths = artifact_paths(root, args.artifacts_file)
        if args.command == "abi":
          result = abi_gate(root, paths, args.sysroot)
        else:
          source_sha = subprocess.check_output(["git", "-C", str(root), "rev-parse", "HEAD"], text=True).strip()
          result = artifact_manifest(root, paths, source_sha)
  except Exception as error:
    # A failed native import or unknown native Params key must leave a failure
    # receipt too; absence of JSON must never resemble an unrun/optional gate.
    result = {"schema": 1, "verdict": "FAIL", "errors": [f"{type(error).__name__}: {error}"]}
  args.output.parent.mkdir(parents=True, exist_ok=True)
  args.output.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
  if result["verdict"] != "PASS":
    for error in result.get("errors", []):
      print(f"{args.command}: {error}", file=sys.stderr)
    for category in ("changed", "added", "removed"):
      if result.get(category):
        print(f"{args.command}: {category}: {', '.join(result[category])}", file=sys.stderr)
  print(f"{args.command}: {result['verdict']} ({args.output})")
  return 0 if result["verdict"] == "PASS" else 1


if __name__ == "__main__":
  raise SystemExit(main())
