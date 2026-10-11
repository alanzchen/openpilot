"""Mazda GEN2 (2019 Mazda3 + TI2) configuration and controller-to-safety checks for the mazda-ti layer.

Params are stubbed BEFORE the car code is imported, so no device Params, shared memory or logs are touched.
Run this file in its own pytest process (it must import the Mazda car modules after the stub is installed).
Scenario expectations are explicit: safety must accept the controller's frames when engaged, and must reject
nonzero torque when controls are not allowed (stale RX, brake, disengaged); 0x220 is characterized as always
accepted (open finding H1, see mazda-ti/PATCHES.md)."""
import sys
import types


class FakeParams:
  store = {"BlendedACC": True, "TorqueInterceptorEnabled": True, "NoMRCC": True, "ManualTransmission": False, "CEStatus": 0}

  def __init__(self, *args, **kwargs):
    pass

  def get_bool(self, key, *args, **kwargs):
    return bool(self.store.get(key, False))

  def get_int(self, key, *args, **kwargs):
    return int(self.store.get(key, 0))

  def get(self, key, *args, **kwargs):
    return self.store.get(key)

  def put(self, *args, **kwargs):
    pass

  def put_bool(self, *args, **kwargs):
    pass


sys.modules["openpilot.common.params"] = types.SimpleNamespace(Params=FakeParams, UnknownKeyName=KeyError)

import pytest  # noqa: E402
from opendbc.car import Bus, structs  # noqa: E402
from opendbc.car.mazda import carcontroller as mazda_cc  # noqa: E402
from opendbc.car.mazda import interface as mazda_if  # noqa: E402
from opendbc.car.mazda.values import CAR, DBC, MazdaSafetyFlags  # noqa: E402
from opendbc.car.structs import CarParams  # noqa: E402
from opendbc.can import CANParser  # noqa: E402,F401
from opendbc.safety.tests.common import CANPackerSafety  # noqa: E402
from opendbc.safety.tests.libsafety import libsafety_py  # noqa: E402

assert mazda_cc.Params is FakeParams and mazda_if.Params is FakeParams, "Params stub must be installed before import"

TI_LKAS, ACC = 0x249, 0x220


def test_alans_configuration_selects_gen2_safety():
  CP = mazda_if.CarInterface.get_non_essential_params(CAR.MAZDA_3_2019)
  assert CP.brand == "mazda"
  assert CP.safetyConfigs[0].safetyModel == CarParams.SafetyModel.mazda
  assert CP.safetyConfigs[0].safetyParam == MazdaSafetyFlags.GEN2   # TI / NoMRCC params do not change GEN2 safety
  assert CP.openpilotLongitudinalControl
  assert CP.steerAtStandstill


class Rig:
  def __init__(self):
    self.CP = mazda_if.CarInterface.get_non_essential_params(CAR.MAZDA_3_2019)
    self.cc = mazda_cc.CarController(DBC[self.CP.carFingerprint], self.CP)
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.mazda, self.CP.safetyConfigs[0].safetyParam)
    self.safety.init_tests()
    self.packer = CANPackerSafety("mazda_2019")
    self.t = 0
    self.CS = types.SimpleNamespace(out=structs.CarState.new_message(), cam_lkas={}, ti_lkas_allowed=True,
                                    acc={"ACCEL_CMD": 2000, "ACC_ENABLED": 1, "HOLD": 0, "RESUME": 0})

  def rx(self, name, bus, values):
    self.safety.safety_rx_hook(self.packer.make_can_msg_safety(name, bus, values))

  def car_inputs(self, engaged=True, brake=0, gas=0, speed=20.0, cruise=True):
    if cruise:
      self.rx("CRUZE_STATE", 0, {"CRZ_STATE": 3 if engaged else 1})
    self.rx("BRAKE_PEDAL_SLOW", 0, {"BRAKE_PEDAL_PRESSED": brake})
    self.rx("ENGINE_DATA", 2, {"PEDAL_GAS": gas})
    self.rx("SPEED", 2, {"SPEED": speed})
    self.rx("TI_FEEDBACK", 1, {"STEER_TORQUE_SENSOR": 0})

  def step(self, lat_active, torque, long_active=False, accel=0.0, **inputs):
    self.t += 10000
    self.safety.set_timer(self.t)
    self.car_inputs(**inputs)
    CC = structs.CarControl.new_message()
    CC.latActive = lat_active
    CC.longActive = long_active
    CC.actuators.torque = torque
    CC.actuators.accel = accel
    self.CS.out.steeringTorque = 0
    _, sends = self.cc.update(CC.as_reader(), self.CS, self.t * 1000, types.SimpleNamespace())
    out = []
    for addr, dat, bus in sends:
      ok = self.safety.safety_tx_hook(libsafety_py.make_CANPacket(addr, bus, dat))
      out.append((addr, bus, dat, ok))
    return out

  def tick(self):
    self.safety.safety_tick_current_safety_config()


def frames(out, addr):
  return [f for f in out if f[0] == addr]


def torque_of(dat):
  v = (dat[0] << 8) | dat[1]
  return v - 0x10000 if v & 0x8000 else v


def test_engaged_steady_state_every_frame_accepted():
  rig = Rig()
  for i in range(300):
    out = rig.step(lat_active=True, torque=0.4 if i < 150 else -0.4)
    for addr, bus, dat, ok in frames(out, TI_LKAS):
      assert bus == 1 and ok, f"frame {i}: TI torque {torque_of(dat)} rejected while engaged"
    assert all(ok for *_, ok in frames(out, ACC))
  assert rig.safety.get_controls_allowed()


def test_disengaged_sends_zero_torque_which_is_accepted():
  rig = Rig()
  for i in range(50):
    out = rig.step(lat_active=False, torque=0.0, engaged=False)
    for addr, bus, dat, ok in frames(out, TI_LKAS):
      assert torque_of(dat) == 0 and ok
  assert not rig.safety.get_controls_allowed()


def test_lateral_request_without_safety_controls_is_rejected():
  rig = Rig()
  rejected = 0
  for i in range(60):
    out = rig.step(lat_active=True, torque=0.4, engaged=False)
    for addr, bus, dat, ok in frames(out, TI_LKAS):
      if torque_of(dat) != 0:
        assert not ok
        rejected += 1
  assert rejected > 0


def test_stale_rx_drops_controls_then_torque_is_rejected():
  rig = Rig()
  for _ in range(100):
    rig.step(lat_active=True, torque=0.3)
  assert rig.safety.get_controls_allowed()
  rig.t += 2_000_000                        # cruise state stops arriving for 2 s
  rig.safety.set_timer(rig.t)
  rig.tick()
  assert not rig.safety.get_controls_allowed()
  out = rig.step(lat_active=True, torque=0.3, cruise=False)
  assert all(not ok for addr, bus, dat, ok in frames(out, TI_LKAS) if torque_of(dat) != 0)


def test_brake_while_moving_disengages_and_torque_is_rejected():
  rig = Rig()
  for _ in range(100):
    rig.step(lat_active=True, torque=0.3)
  out = rig.step(lat_active=True, torque=0.3, brake=1)
  assert not rig.safety.get_controls_allowed()
  out = rig.step(lat_active=True, torque=0.3, brake=1)
  assert all(not ok for addr, bus, dat, ok in frames(out, TI_LKAS) if torque_of(dat) != 0)


def test_gas_does_not_disengage():
  rig = Rig()
  for _ in range(100):
    rig.step(lat_active=True, torque=0.3)
  out = rig.step(lat_active=True, torque=0.3, gas=100)
  assert rig.safety.get_controls_allowed()
  assert all(ok for *_, ok in frames(out, TI_LKAS))


@pytest.mark.parametrize("ce_status, long_active, expect_op", [(1, True, True), (0, True, False), (1, False, False)])
def test_blended_acc_selection_and_acc_frames_always_accepted(ce_status, long_active, expect_op):
  FakeParams.store["CEStatus"] = ce_status
  try:
    rig = Rig()
    accel = -1.0
    seen = []
    for _ in range(80):
      out = rig.step(lat_active=True, torque=0.0, long_active=long_active, accel=accel)
      seen += frames(out, ACC)
    assert seen and all(ok for *_, ok in seen)   # H1: 0x220 has no tx-hook check (characterization, not a pass)
    last = rig.CS.acc["ACCEL_CMD"]
    if expect_op:
      assert last < 2000, "BlendedACC with CEStatus!=0 and longActive must follow openpilot's (negative) accel"
    else:
      assert last == pytest.approx(2000, abs=1), "otherwise the stock ACCEL_CMD passes through"
  finally:
    FakeParams.store["CEStatus"] = 0
