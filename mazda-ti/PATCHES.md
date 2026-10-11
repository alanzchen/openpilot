# mazda-ti: Mazda / torque-interceptor layer on official StarPilot

This branch is **official StarPilot** (firestar5683/StarPilot, branch `StarPilot`) plus a small, explicit
adaptation layer for Mazda cars with a MoreTorque torque interceptor (TI/TI2), originally developed by MoreTore
(MoreTore/openpilot `StarPilot-dev`). Upstream history is kept intact; the layer is a linear series of commits
on top, and later official updates are merged in (never rebased or squashed).

- Official base: `2a12dbd0ad94b46f8a7b6099d32d7216b7676f79` (recorded in `mazda-ti/UPSTREAM`).
- MoreTore source: `c183c4c2535ce4a39d0459265bb83d37e10e19c3` (equivalent to official `a33bec1` + the Mazda port).
- Validated target: 2019 Mazda3 (GEN2) + TI2 on a comma 3 (tici). Other Mazda configurations are carried but
  not vehicle-tested here.

## Layer commits (in order)
| Commit | What | Sources |
|---|---|---|
| Mazda car port | `opendbc/car/mazda/*`, `dbc/mazda_{2017,2019,2023}.dbc`, `car/fingerprints.py` (CX-8) taken whole; torque data and `can/dbc.py` Mazda checksums as hunks; debug print removed; `Params(memory=True)`; 6 untested platforms listed in `routes.py` | MoreTore 1d5d6fb 8932b10 f7d9ca0 b1771f2 6ab3c05 fe00f46 f52db1e 5b9021e 558f816 e9320b2 e2d44d8 c7ffee1 26e0a7b 65c68a7 467f2f3 |
| Mazda safety | `opendbc/safety/modes/mazda.h` whole; panda `main.c` SAFETY_MAZDA relay/CAN mode; `can_common.h` CAN ignition 0x274 | MoreTore d69724e 21e51ed 8fd7a2d 20ca825 e2d44d8 |
| Params + toggles | 6 Mazda keys in `params_keys.h`; Mazda toggles (StarPilot vehicle, classic, mici); `isMazda`; BlendedACC hook in `longcontrol.py` limited to Mazda | MoreTore 80ba40c e2d44d8 df1df10 |
| 2019 Mazda3 firmware | recorded FW versions + exact-match test | alanzchen 03155ba (cherry-picked) |
| pandad (tici) | udev wait before listing, re-enumeration wait, internal-panda pre-check, action-based recovery stages, USB-only listing on tici | concept: official SecretGoodStarPilot 348b357, 0669069 (ported) |
| Tests | `test_mazda_ti.py`, GEN1-flag setup in `test_mazda.py`, TestMazda* cross-mode exclusion, controller-to-safety integration test, longcontrol Mazda-only test | new |
| Tooling | this ledger, `mazda-ti/UPSTREAM`, policy, sync/stamp/rebuild scripts and their tests, workflow reference copy | new |
| tici artifacts | every scons-produced tracked binary rebuilt on a comma 3 from this exact source; `mazda-ti/BUILD-STAMP.json` | built on device |

## Deliberately not taken from MoreTore / the old branch
- `selfdrive/controls/controlsd.py`, `lib/drive_helpers.py` (+test): MoreTore's copies predate and revert an
  official Kona fix.
- `selfdrive/ui/layouts/settings/developer.py`: the maneuver toggles and 17 `put_bool(..., block=True)` calls
  that crashed the comma 3 UI; the official file is used, so the old Params hotfix (3ea7dcc) is unnecessary.
- Galaxy power monitor and maneuver JSON changes, `launch_onroad_desktop.sh`, `tools/lateral_maneuvers/*`,
  `system/version.py` fallback, MoreTore's default flips (AlwaysAllowUploads, UseKonikServer, *LateralTune*),
  two Hyundai hotfixes, and every MoreTore/official prebuilt binary.
- Old branch commits 34fd36d (already upstream) and b1050ba (test tweak for MoreTore's Galaxy frontend).

## Open safety findings (preserved, NOT fixed; need vehicle validation)
Pinned by `expectedFailure` tests in `test_mazda_ti.py`. An unexpected success means the fix must remove the marker.
- **H1** GEN2/GEN3 ACC `0x220` (bus 2) is allowlisted but has no tx-hook check (no controls gate, no accel bound).
  The controller sends it every 2 frames, also outside openpilot longitudinal, rewriting HOLD/RESUME.
- **M1** GEN2: every `CRUZE_STATE` frame sets `acc_main_on`, so the always-on-lateral main-switch predicate is
  always asserted (steering still requires the AOL alternative experience and torque limits).
- **H2** GEN1 + TI: TI torque `0x249` (bus 1) is allowlisted but never limited (not this car's configuration).
- Also noted: panda driver-torque blending is 5x looser than the controller; TI health is not mapped to
  steerFault; safety brake source (0x43F) differs from carstate (0x9F); 0x274 ignition has no counter/checksum.

## Tests and what they establish
- Safety: GEN2/TI characterization, param-0 fail-safe, cross-generation rejections, GEN1 suite.
- Differential: identical seeded rx/tx/timer sequences give identical outcomes for MoreTore c183c4c and this
  branch (equivalence for the tested sequences only).
- Controller-to-safety: explicit accept/reject expectations per scenario for the 2019 Mazda3 configuration.
- Software tests are not evidence of safe physical behavior; vehicle validation is the owner's step.

## Keeping up with official StarPilot
- `.github/workflows/starpilot-autosync.yml` on the default branch (daily + manual) runs the trusted copies in
  `.github/mazda-ti-sync/`.
  - It merges new official commits into `autosync/starpilot` (fast-forward pushes only), runs the tests above,
    classifies the result and opens/updates one draft PR into `mazda-starpilot-ti`.
  - It never merges, deploys or flashes.
- Verdicts (`mazda-ti/sync/classify.py`):
  - `MANUAL_REVIEW`: official touched a layer file, the AGNOS pin, launch scripts or workflows, or a required hunk is gone;
  - `NEEDS_DEVICE_REBUILD`: the tici build stamp does not verify for this exact SHA;
  - `READY`.
  - Stops: upstream history rewrite, any source conflict, a closed unmerged candidate PR.
- To make a candidate deployable, run `mazda-ti/sync/device_rebuild.sh <repo> <branch> <policy>` on a comma 3.
  It builds in a fresh worktree, commits the artifacts with `BUILD-STAMP.json`, and verifies them.
- Deployment and vehicle validation remain manual.
