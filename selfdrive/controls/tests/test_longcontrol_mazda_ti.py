"""mazda-ti: the BlendedACC PID-reset hook in LongControl only exists for Mazda.

Params is stubbed before import so no device Params are touched. Run with --noconftest."""
import sys
import types

created = []


class FakeParams:
  def __init__(self, *args, **kwargs):
    created.append(kwargs)

  def get_bool(self, key, *args, **kwargs):
    return True

  def get_int(self, key, *args, **kwargs):
    return 1


sys.modules["openpilot.common.params"] = types.SimpleNamespace(Params=FakeParams, UnknownKeyName=KeyError)

from cereal import car  # noqa: E402
from openpilot.selfdrive.controls.lib import longcontrol  # noqa: E402

assert longcontrol.Params is FakeParams


def make(brand):
  CP = car.CarParams.new_message()
  CP.brand = brand
  CP.longitudinalTuning.kpBP = [0.]
  CP.longitudinalTuning.kpV = [1.]
  CP.longitudinalTuning.kiBP = [0.]
  CP.longitudinalTuning.kiV = [0.1]
  return longcontrol.LongControl(CP.as_reader())


def test_non_mazda_has_no_blended_acc_hook():
  created.clear()
  lc = make("toyota")
  assert lc.mazda_blended_acc is False
  assert created == [], "non-Mazda LongControl must not read BlendedACC/CEStatus"


def test_mazda_has_blended_acc_hook():
  created.clear()
  lc = make("mazda")
  assert lc.mazda_blended_acc is True
  assert {"memory": True} in created
