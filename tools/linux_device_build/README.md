# Linux device build

The `Linux device build` Actions workflow runs on native `ubuntu-24.04-arm`.
It builds device artifacts inside the hash-verified AGNOS 19.6.20 root filesystem,
using its Python 3.12.3, compiler and managed native libraries. Ubuntu is the
runner host; Ubuntu host libraries do not enter the device binaries.

The pipeline checks out the exact PR source and independently rebuilds two clean
copies at `/work` in disposable containers. Both rebuilds have read-only roots,
no network, no build cache and a read-only copy of the gate scripts. Only the
source checkout, temporary directories and reports are writable.

Required gates are:

- Pinned sparse/raw AGNOS image hashes and unchanged managed dependency bytes.
- Rebuilding every artifact in the previously validated deployment inventory.
- Identical artifact paths, modes and bytes across both clean rebuilds.
- No staged or unstaged source changes outside that generated-artifact inventory.
- AArch64 runtime ELF dependencies and symbol-version compatibility with AGNOS;
  Panda firmware ELF files are checked as ARM32.
- Direct loading of the rebuilt native Params extension, checking its complete
  registry against source and persisting the gaze settings through that backend.
- Native Params, Mazda safety/car/control, screen policy, onboard settings and
  Galaxy acceptance tests with logs, JUnit reports and a JSON verdict.

GPU/OpenCL execution, physical cameras, Panda flashing and vehicle behavior are
outside this CPU builder's coverage. The old Mac builder's private source was
not available; the Linux receipt states its own gates rather than claiming
unverified parity with that implementation.

After every gate passes, Actions uploads the artifact bundle and reports. A
separate publishing job, restricted to branches in this repository, verifies the
bundle against the exact source SHA and immutable deployment-path allowlist. It
also checks staged Git blobs, so attributes or filters cannot silently change
validated binary bytes. It commits only generated outputs, an empty `prebuilt`
marker and `build_stamps/linux-device-build.json`. A concurrent source push
rejects publication. Successful publication posts `linux-device-build` on the
new artifact commit, with the Actions run linked as its evidence.

This publication step matters: this fork tracks its prebuilts. Uploading CI
artifacts alone leaves the installed Params binary out of sync with new keys,
which causes settings saves to fail with `UnknownKeyName`.

The image pins live in `environment/pins.json`; build/test tool versions and
wheel hashes come from `uv.lock`. Do not update the required branch check until
a complete run, publication and native registry check have passed.

To run the same orchestration on a native Linux ARM64 Docker host:

```sh
export LINUX_DEVICE_WORK_ROOT=/path/outside/the/checkout/linux-device-build
export LINUX_DEVICE_JOBS=4
bash tools/linux_device_build/run.sh
```

The host needs Git, Python 3.11+, Docker, `debugfs`, `simg2img` and enough free
space for the device root and two build trees. The workflow installs the two
filesystem tools via `e2fsprogs` and `android-sdk-libsparse-utils`.

Run policy/gate unit tests on a development host with:

```sh
python -m pytest -q -c /dev/null --confcutdir=tools/linux_device_build \
  tools/linux_device_build/test_gates.py tools/linux_device_build/tests/test_package.py
```
