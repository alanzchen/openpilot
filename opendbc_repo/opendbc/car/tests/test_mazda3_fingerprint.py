import pytest

from opendbc.car.fw_versions import match_fw_to_car
from opendbc.car.mazda.values import CAR
from opendbc.car.structs import CarParams
from opendbc.car.vin import VIN_UNKNOWN


# Firmware observed on a 2019 Mazda 3, also recorded by the Mazda FrogPilot fork.
MAZDA3_FW = (
  (CarParams.Ecu.engine, 0x7e0, b"PX13-188K2-H"),
  (CarParams.Ecu.transmission, 0x7e1, b"PX01-21PS1-E"),
  (CarParams.Ecu.fwdCamera, 0x706, b"BDGF-67WK2-C"),
  (CarParams.Ecu.eps, 0x730, b"BDGF-3216X-B"),
  (CarParams.Ecu.fwdRadar, 0x764, b"BDTS-67XK2-A"),
  (CarParams.Ecu.abs, 0x760, b"BCKA-4300F-E"),
)


@pytest.mark.parametrize("unknown_engine", [False, True])
def test_mazda3_recorded_firmware(unknown_engine):
  firmware = [
    CarParams.CarFw(ecu=ecu, address=address, brand="mazda", fwVersion=version.ljust(24, b"\x00"))
    for ecu, address, version in MAZDA3_FW
  ]
  if unknown_engine:
    firmware[0].fwVersion = b"UNKNOWN".ljust(24, b"\x00")

  exact, matches = match_fw_to_car(firmware, VIN_UNKNOWN, allow_fuzzy=False, log=False)
  assert exact
  assert matches == (set() if unknown_engine else {CAR.MAZDA_3_2019})
