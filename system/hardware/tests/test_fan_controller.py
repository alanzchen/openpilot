import pytest

from openpilot.system.hardware.fan_controller import TiciFanController

ALL_CONTROLLERS = [TiciFanController]


def test_aggressive_cooling_is_optional_and_keeps_offroad_limit(mocker):
  mocker.patch("openpilot.system.hardware.fan_controller.IS_MICI", False)
  stock = patched_controller(mocker, TiciFanController)
  aggressive = patched_controller(mocker, TiciFanController)
  assert aggressive.update(75, True, True) > stock.update(75, True)
  assert aggressive.update(100, False, True) <= 30
  assert aggressive.update(75, True, False) == TiciFanController().update(75, True)


def test_mici_ignores_aggressive_cooling(mocker):
  mocker.patch("openpilot.system.hardware.fan_controller.IS_MICI", True)
  stock = patched_controller(mocker, TiciFanController)
  aggressive = patched_controller(mocker, TiciFanController)
  assert aggressive.update(75, True, True) == stock.update(75, True)

def patched_controller(mocker, controller_class):
  mocker.patch("os.system", new=mocker.Mock())
  return controller_class()

class TestFanController:
  def wind_up(self, controller, ignition=True):
    for _ in range(1000):
      controller.update(100, ignition)

  def wind_down(self, controller, ignition=False):
    for _ in range(1000):
      controller.update(10, ignition)

  @pytest.mark.parametrize("controller_class", ALL_CONTROLLERS)
  def test_hot_onroad(self, mocker, controller_class):
    controller = patched_controller(mocker, controller_class)
    self.wind_up(controller)
    assert controller.update(100, True) >= 70

  @pytest.mark.parametrize("controller_class", ALL_CONTROLLERS)
  def test_offroad_limits(self, mocker, controller_class):
    controller = patched_controller(mocker, controller_class)
    self.wind_up(controller)
    assert controller.update(100, False) <= 30

  @pytest.mark.parametrize("controller_class", ALL_CONTROLLERS)
  def test_no_fan_wear(self, mocker, controller_class):
    controller = patched_controller(mocker, controller_class)
    self.wind_down(controller)
    assert controller.update(10, False) == 0

  @pytest.mark.parametrize("controller_class", ALL_CONTROLLERS)
  def test_limited(self, mocker, controller_class):
    controller = patched_controller(mocker, controller_class)
    self.wind_up(controller, True)
    assert controller.update(100, True) == 100

  @pytest.mark.parametrize("controller_class", ALL_CONTROLLERS)
  def test_windup_speed(self, mocker, controller_class):
    controller = patched_controller(mocker, controller_class)
    self.wind_down(controller, True)
    for _ in range(10):
      controller.update(90, True)
    assert controller.update(90, True) >= 60
