# mazda-ti: Mazda / torque-interceptor layer on official StarPilot

This branch is **official StarPilot** (firestar5683/StarPilot, branch `StarPilot`) plus a small, explicit
adaptation layer for Mazda cars with a MoreTorque torque interceptor (TI/TI2), originally developed by MoreTore
(MoreTore/openpilot `StarPilot-dev`). Upstream history is kept intact; the layer is a linear series of commits
on top, and later official updates are merged in (never rebased or squashed). Scope: Mazda + TI support only.

- Official base: `2a12dbd0ad94b46f8a7b6099d32d7216b7676f79`.
- MoreTore source: `c183c4c2535ce4a39d0459265bb83d37e10e19c3` (equivalent to official `a33bec1` + the Mazda port).
- Original deployed Mazda tree: fork branch `mazda-starpilot` (`b1050ba5`). `opendbc/safety/modes/mazda.h`,
  `opendbc/car/mazda/{mazdacan,carstate,values,fingerprints,radar_interface}.py` and the three Mazda DBCs are
  byte-identical to it (and `mazda.h` to MoreTore `c183c4c`).
- Target: 2019 Mazda3 (GEN2) + TI2 on a comma 3 (tici). Other Mazda configurations are carried but not
  vehicle-tested here.

## Layer commits (in order)
| Commit | What | Sources |
|---|---|---|
| Mazda car port | `opendbc/car/mazda/*`, `dbc/mazda_{2017,2019,2023}.dbc`, `car/fingerprints.py` (CX-8) taken whole; torque data and `can/dbc.py` Mazda checksums as hunks; debug print removed; `Params(memory=True)`; 6 untested platforms listed in `routes.py` | MoreTore 1d5d6fb 8932b10 f7d9ca0 b1771f2 6ab3c05 fe00f46 f52db1e 5b9021e 558f816 e9320b2 e2d44d8 c7ffee1 26e0a7b 65c68a7 467f2f3 |
| Mazda safety | `opendbc/safety/modes/mazda.h` whole; panda `main.c` SAFETY_MAZDA relay/CAN mode; `can_common.h` CAN ignition 0x274 | MoreTore d69724e 21e51ed 8fd7a2d 20ca825 e2d44d8 |
| Params + toggles | 6 Mazda keys in `params_keys.h`; Mazda toggles (StarPilot vehicle, classic, mici); `isMazda`; BlendedACC hook in `longcontrol.py` limited to Mazda | MoreTore 80ba40c e2d44d8 df1df10 |
| 2019 Mazda3 firmware | recorded FW versions + exact-match test | alanzchen 03155ba (cherry-picked) |
| Tests | `test_mazda_ti.py`, GEN1-flag setup in `test_mazda.py`, TestMazda* cross-mode exclusion, controller-to-safety integration test, longcontrol Mazda-only test | new |
| This ledger | `mazda-ti/PATCHES.md` | new |

Removed again from the layer (not Mazda/TI support): the comma 3 pandad udev-wait change (45cf83f1, reverted;
pandad is official) and the earlier on-device sync/stamp/rebuild tooling (superseded; builds and upstream sync
are run outside this tree by the owner's trusted Mac builder, whose stamp is `build_stamps/mac-larch64.json`).

## Deliberately not taken from MoreTore / the old branch
- `selfdrive/controls/controlsd.py`, `lib/drive_helpers.py` (+test): MoreTore's copies predate and revert an
  official Kona fix.
- `selfdrive/ui/layouts/settings/developer.py`: the maneuver toggles and 17 `put_bool(..., block=True)` calls
  that crashed the comma 3 UI; the official file is used, so the old Params hotfix (3ea7dcc) is unnecessary.
- Galaxy power monitor and maneuver JSON changes, `launch_onroad_desktop.sh`, `tools/lateral_maneuvers/*`,
  `system/version.py` fallback, MoreTore's default flips (AlwaysAllowUploads, UseKonikServer, *LateralTune*),
  two Hyundai hotfixes, and every MoreTore/official prebuilt binary.
- Old branch commits 34fd36d (already upstream) and b1050ba (test tweak for MoreTore's Galaxy frontend).

## Open safety findings / Safety follow-ups (inherited, preserved, NOT fixed; need vehicle validation)
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
- Controller-to-safety: explicit accept/reject expectations per scenario for the 2019 Mazda3 configuration.
- Software tests are not evidence of safe physical behavior; vehicle validation is the owner's step.
- Deployment, firmware flashing and vehicle validation are manual and separately authorized.
