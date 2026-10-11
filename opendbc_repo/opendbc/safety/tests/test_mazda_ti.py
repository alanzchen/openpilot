#!/usr/bin/env python3
"""Mazda safety with MoreTore's generation and torque-interceptor flags (mazda-ti layer).

The official test_mazda.py exercises param 0, which this mode treats as "no generation" (nothing is parsed,
controls are never allowed). These tests pin the flagged configurations instead:
- test_mazda.py (official GEN1 suite) runs with FLAG_GEN1; TestMazdaUnconfiguredParam pins param 0 as fail-safe.
- TestMazda2019TISafety: GEN2 (2019+ Mazda3 class + TI2), the configuration of this fork's car.
Known open findings are expectedFailure tests; they turn green only when the safety code is fixed and
vehicle-validated (see mazda-ti/PATCHES.md, "Safety follow-ups")."""
import unittest

from opendbc.car.mazda.values import MazdaSafetyFlags
from opendbc.car.structs import CarParams
from opendbc.safety import ALTERNATIVE_EXPERIENCE
from opendbc.safety.tests.libsafety import libsafety_py
import opendbc.safety.tests.common as common
from opendbc.safety.tests.common import CANPackerSafety

MAZDA_AUX = 1
MAZDA_CAM = 2
TI_LKAS = 0x249
ACC_2019 = 0x220


class TestMazda2019TISafety(common.CarSafetyTest, common.DriverTorqueSteeringSafetyTest):
  FLAGS = MazdaSafetyFlags.GEN2

  TX_MSGS = [[TI_LKAS, MAZDA_AUX], [ACC_2019, MAZDA_CAM]]
  STANDSTILL_THRESHOLD = .1
  RELAY_MALFUNCTION_ADDRS = {MAZDA_CAM: (ACC_2019,)}
  FWD_BLACKLISTED_ADDRS = {0: [ACC_2019]}

  MAX_RATE_UP = 45
  MAX_RATE_DOWN = 80
  MAX_TORQUE_LOOKUP = [0], [8000]
  MAX_RT_DELTA = 1688

  DRIVER_TORQUE_ALLOWANCE = 1400
  DRIVER_TORQUE_FACTOR = 1

  # the TI torque message has no request bit
  NO_STEER_REQ_BIT = True

  def setUp(self):
    self.packer = CANPackerSafety("mazda_2019")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.mazda, self.FLAGS)
    self.safety.init_tests()

  def _torque_meas_msg(self, torque):
    return self._torque_driver_msg(torque)

  def _torque_driver_msg(self, torque):
    return self.packer.make_can_msg_safety("TI_FEEDBACK", MAZDA_AUX, {"STEER_TORQUE_SENSOR": torque})

  def _torque_cmd_msg(self, torque, steer_req=1):
    # mazda.h reads bytes 0-1 as int16; LKAS_REQUEST is declared unsigned, so pack two's complement
    return self.packer.make_can_msg_safety("EPS_LKAS", MAZDA_AUX, {"LKAS_REQUEST": int(torque) & 0xFFFF, "STEER_FEEL": 10000})

  def _speed_msg(self, speed):
    return self.packer.make_can_msg_safety("SPEED", MAZDA_CAM, {"SPEED": speed})

  def _user_brake_msg(self, brake):
    return self.packer.make_can_msg_safety("BRAKE_PEDAL_SLOW", 0, {"BRAKE_PEDAL_PRESSED": brake})

  def _user_gas_msg(self, gas):
    return self.packer.make_can_msg_safety("ENGINE_DATA", MAZDA_CAM, {"PEDAL_GAS": gas})

  def _pcm_status_msg(self, enable):
    return self.packer.make_can_msg_safety("CRUZE_STATE", 0, {"CRZ_STATE": 3 if enable else 1})

  def _acc_msg(self, accel_cmd):
    return self.packer.make_can_msg_safety("ACC", MAZDA_CAM, {"ACCEL_CMD": accel_cmd})

  def test_cruise_state_threshold(self):
    for crz_state in range(8):
      self.safety.set_controls_allowed(False)
      self._rx(self.packer.make_can_msg_safety("CRUZE_STATE", 0, {"CRZ_STATE": 0}))
      self._rx(self.packer.make_can_msg_safety("CRUZE_STATE", 0, {"CRZ_STATE": crz_state}))
      self.assertEqual(crz_state >= 2, self.safety.get_controls_allowed(), f"{crz_state=}")

  def test_signed_driver_torque(self):
    for _ in range(6):
      self._rx(self._torque_driver_msg(-1500))
    self.assertEqual(-1500, self.safety.get_torque_driver_min())
    self.assertEqual(-1500, self.safety.get_torque_driver_max())

  def test_gen1_messages_ignored(self):
    self.safety.set_controls_allowed(False)
    gen1 = CANPackerSafety("mazda_2017")
    for bus in (0, MAZDA_AUX):
      self._rx(gen1.make_can_msg_safety("CRZ_CTRL", bus, {"CRZ_ACTIVE": 1, "CRZ_AVAILABLE": 1}))
    self._rx(gen1.make_can_msg_safety("STEER_TORQUE", 0, {"STEER_TORQUE_SENSOR": 500}))
    self.assertFalse(self.safety.get_controls_allowed())
    self.assertEqual(0, self.safety.get_torque_driver_max())

  def _rx_check_msgs(self):
    return {
      "BRAKE_PEDAL_SLOW": lambda: self._user_brake_msg(0),
      "ENGINE_DATA": lambda: self._user_gas_msg(0),
      "CRUZE_STATE": lambda: self._pcm_status_msg(True),
      "SPEED": lambda: self._speed_msg(0),
      "TI_FEEDBACK": lambda: self._torque_driver_msg(0),
    }

  def _lag_scenario(self, missing):
    self.setUp()
    msgs = self._rx_check_msgs()
    self.safety.set_timer(0)
    for make in msgs.values():
      self._rx(make())
    self.safety.set_controls_allowed(True)
    # 2 s later (beyond every 1 s rx-check window) all messages except `missing` are still arriving
    self.safety.set_timer(int(2e6))
    for name, make in msgs.items():
      if name != missing:
        self._rx(make())
    self.safety.safety_tick_current_safety_config()
    return self.safety.get_controls_allowed()

  def test_rx_check_fresh_messages_keep_controls(self):
    self.assertTrue(self._lag_scenario(missing=None))

  def test_rx_check_lag_disables_controls(self):
    for missing in self._rx_check_msgs():
      self.assertFalse(self._lag_scenario(missing), f"controls stayed allowed with {missing} lagging")

  def test_acc_tx_allowed_when_engaged(self):
    self.safety.set_controls_allowed(True)
    self.assertTrue(self._tx(self._acc_msg(2000)))

  @unittest.expectedFailure
  def test_acc_blocked_without_controls(self):
    # Open finding H1: GEN2 0x220 is allowlisted but has no tx-hook check, so arbitrary ACCEL_CMD passes
    # while controls are not allowed. Fixing it needs a design that keeps stock passthrough (vehicle test).
    self.safety.set_controls_allowed(False)
    self.assertFalse(self._tx(self._acc_msg(0)))

  def _toggle_aol(self, toggle_on):
    return self.packer.make_can_msg_safety("CRUZE_STATE", 0, {"CRZ_STATE": 1 if toggle_on else 0})

  @unittest.expectedFailure
  def test_always_on_lateral(self):
    # Open finding M1: every CRUZE_STATE frame sets acc_main_on, so the AOL main-switch gate never closes.
    super().test_always_on_lateral()


class TestMazda2019TIFlagEquivalence(TestMazda2019TISafety):
  # interface.py only emits GEN2 for the 2019+ platforms; extra TI / NO_MRCC flags must not change GEN2 safety
  FLAGS = MazdaSafetyFlags.GEN2 | MazdaSafetyFlags.TORQUE_INTERCEPTOR


class TestMazda2019NoMrccFlagEquivalence(TestMazda2019TISafety):
  FLAGS = MazdaSafetyFlags.GEN2 | MazdaSafetyFlags.NO_MRCC


class TestMazdaGen1TISafety(unittest.TestCase):
  """GEN1 + torque interceptor (not this fork's car; kept so the gap stays visible)."""
  TX_MSGS = None

  def setUp(self):
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.mazda, MazdaSafetyFlags.GEN1 | MazdaSafetyFlags.TORQUE_INTERCEPTOR)
    self.safety.init_tests()
    self.packer = CANPackerSafety("mazda_2019")

  @unittest.expectedFailure
  def test_ti_torque_requires_controls(self):
    # Open finding H2: with GEN1|TI the 0x249 bus-1 torque command is allowlisted but never limited.
    self.safety.set_controls_allowed(False)
    msg = self.packer.make_can_msg_safety("EPS_LKAS", MAZDA_AUX, {"LKAS_REQUEST": 8000, "STEER_FEEL": 10000})
    self.assertFalse(self.safety.safety_tx_hook(msg))


class TestMazdaUnconfiguredParam(unittest.TestCase):
  """param 0 selects no generation: nothing is parsed, controls are never allowed, steering is blocked."""
  TX_MSGS = None

  def setUp(self):
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.mazda, 0)
    self.safety.init_tests()

  def test_never_allows_controls(self):
    gen1 = CANPackerSafety("mazda_2017")
    gen2 = CANPackerSafety("mazda_2019")
    for _ in range(3):
      self.safety.safety_rx_hook(gen1.make_can_msg_safety("CRZ_CTRL", 0, {"CRZ_ACTIVE": 1, "CRZ_AVAILABLE": 1}))
      self.safety.safety_rx_hook(gen2.make_can_msg_safety("CRUZE_STATE", 0, {"CRZ_STATE": 3}))
    self.assertFalse(self.safety.get_controls_allowed())
    # LKAS_REQUEST carries the -2048 offset in the DBC, so this is a small (100) torque request
    self.assertFalse(self.safety.safety_tx_hook(gen1.make_can_msg_safety("CAM_LKAS", 0, {"LKAS_REQUEST": 100})))

  def test_zero_torque_characterization(self):
    # with no generation selected the default GEN1 TX list remains, so a zero-torque LKAS frame is accepted
    gen1 = CANPackerSafety("mazda_2017")
    self.assertTrue(self.safety.safety_tx_hook(gen1.make_can_msg_safety("CAM_LKAS", 0, {"LKAS_REQUEST": 0})))


class TestMazdaCrossGenerationTx(unittest.TestCase):
  """The TestMazda* exclusion in test_tx_hook_on_wrong_safety_mode is compensated here: each Mazda configuration
  must reject the other generations' control messages."""
  TX_MSGS = None

  def _hooks(self, flags):
    safety = libsafety_py.libsafety
    safety.set_safety_hooks(CarParams.SafetyModel.mazda, flags)
    safety.init_tests()
    safety.set_controls_allowed(True)
    return safety

  def test_gen2_rejects_gen1_messages(self):
    safety = self._hooks(MazdaSafetyFlags.GEN2)
    gen1 = CANPackerSafety("mazda_2017")
    self.assertFalse(safety.safety_tx_hook(gen1.make_can_msg_safety("CAM_LKAS", 0, {"LKAS_REQUEST": 0})))
    self.assertFalse(safety.safety_tx_hook(gen1.make_can_msg_safety("CRZ_BTNS", 0, {"CAN_OFF": 1, "CAN_OFF_INV": 0})))
    self.assertFalse(safety.safety_tx_hook(gen1.make_can_msg_safety("CAM_LANEINFO", 0, {})))

  def test_gen1_without_ti_rejects_gen2_messages(self):
    safety = self._hooks(MazdaSafetyFlags.GEN1)
    gen2 = CANPackerSafety("mazda_2019")
    self.assertFalse(safety.safety_tx_hook(gen2.make_can_msg_safety("EPS_LKAS", MAZDA_AUX, {"LKAS_REQUEST": 0, "STEER_FEEL": 10000})))
    self.assertFalse(safety.safety_tx_hook(gen2.make_can_msg_safety("ACC", MAZDA_CAM, {"ACCEL_CMD": 2000})))

  def test_gen2_ti_message_only_on_aux_bus(self):
    safety = self._hooks(MazdaSafetyFlags.GEN2)
    gen2 = CANPackerSafety("mazda_2019")
    for bus in (0, MAZDA_CAM):
      self.assertFalse(safety.safety_tx_hook(gen2.make_can_msg_safety("EPS_LKAS", bus, {"LKAS_REQUEST": 0, "STEER_FEEL": 10000})))


if __name__ == "__main__":
  unittest.main()
