#!/usr/bin/env python3
"""Differential check of the Mazda safety mode between two source trees (e.g. MoreTore c183c4c vs mazda-ti).

  safety_differential.py --tree A --tree B [--seed 1] [--steps 4000]

Each tree's opendbc libsafety (build it first: scons opendbc/safety/tests/libsafety/libsafety.so) runs the same
seeded sequence of rx/tx/timer operations for several Mazda parameter sets in its own process; every return value and
the observable safety state must match. This establishes equivalence only for the sequences tested.
"""
import argparse
import json
import os
import subprocess
import sys

CHILD = r'''
import json, random, sys
from opendbc.safety.tests.libsafety import libsafety_py
from opendbc.car.structs import CarParams
seed, steps = int(sys.argv[1]), int(sys.argv[2])
s = libsafety_py.libsafety
RX = [(0x43F, 0), (0x202, 2), (0x44A, 0), (0x217, 2), (0x24B, 1), (0x21C, 0), (0x21C, 1), (0x165, 0), (0x240, 0),
      (0x24A, 1), (0x09D, 0), (0x9F, 0), (0x215, 0), (0x215, 2), (0x44A, 1), (0x220, 0), (0x249, 1)]
TX = [(0x249, 1), (0x220, 2), (0x243, 0), (0x09D, 0), (0x440, 0), (0x21C, 0), (0x249, 0), (0x220, 0)]
out = []
for param in (0, 1, 2, 4, 8 | 1, 8 | 2, 64 | 2, 16 | 1, 16 | 8 | 1):
  rnd = random.Random(seed * 1000 + param)
  s.set_safety_hooks(CarParams.SafetyModel.mazda, param)
  s.init_tests()
  t = 0
  for i in range(steps):
    op = rnd.random()
    data = bytes(rnd.choice([0, 0, 0x20, 0x40, 0x60, 0xff, rnd.randrange(256)]) for _ in range(8))
    if op < 0.55:
      addr, bus = rnd.choice(RX)
      r = s.safety_rx_hook(libsafety_py.make_CANPacket(addr, bus, data))
      kind = "rx"
    elif op < 0.9:
      addr, bus = rnd.choice(TX)
      r = s.safety_tx_hook(libsafety_py.make_CANPacket(addr, bus, data))
      kind = "tx"
    elif op < 0.97:
      t += rnd.choice([10000, 50000, 200000, 1500000])
      s.set_timer(t)
      s.safety_tick_current_safety_config()
      addr = bus = r = None
      kind = "tick"
    else:
      s.set_controls_allowed(rnd.random() < 0.5)
      addr = bus = r = None
      kind = "set_controls"
    out.append([param, i, kind, addr, bus, bool(r) if r is not None else None, s.get_controls_allowed(),
                s.get_relay_malfunction(), s.get_torque_driver_min(), s.get_torque_driver_max(),
                s.get_vehicle_moving(), s.get_gas_pressed_prev(), s.get_brake_pressed_prev()])
print(json.dumps(out))
'''


def run(tree, seed, steps):
  env = dict(os.environ, PYTHONPATH=os.path.join(tree, "opendbc_repo"))
  p = subprocess.run([sys.executable, "-c", CHILD, str(seed), str(steps)], env=env, capture_output=True, text=True,
                     cwd=os.path.join(tree, "opendbc_repo"), check=True)
  return json.loads(p.stdout)


def main():
  ap = argparse.ArgumentParser()
  ap.add_argument("--tree", action="append", required=True)
  ap.add_argument("--seed", type=int, default=1)
  ap.add_argument("--steps", type=int, default=4000)
  a = ap.parse_args()
  assert len(a.tree) == 2
  ra, rb = (run(t, a.seed, a.steps) for t in a.tree)
  diffs = [(x, y) for x, y in zip(ra, rb) if x != y]
  print(json.dumps({"records": len(ra), "differences": len(diffs), "first": diffs[:3]}))
  sys.exit(1 if diffs or len(ra) != len(rb) else 0)


if __name__ == "__main__":
  main()
