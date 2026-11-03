import math
from dataclasses import replace

import pytest
from cereal import log

from openpilot.starpilot.common.screen_gaze import ScreenGazeDimmer, screen_yaw
from openpilot.starpilot.common.screen_settings import GazeDimmingPreferences


def monitoring_state(yaw_degrees=0, *, rhd=False, neutral=0.075, calibrated=False):
  dm = log.DriverMonitoringState.new_message()
  dm.activePolicy = 'vision'
  dm.isRHD = rhd
  dm.visionPolicyState.faceDetected = True
  pose = dm.visionPolicyState.pose
  pose.calibrated = calibrated
  pose.yawCalib.offset = neutral
  pose.yaw = neutral + math.radians(yaw_degrees) * (-1 if rhd else 1)
  pose.uncertainty = 0.1
  return dm


@pytest.mark.parametrize('rhd', [False, True])
@pytest.mark.parametrize('physical_yaw', [-25, 0, 25])
@pytest.mark.parametrize('side,direction', [(0, 1), (1, -1)])
def test_physical_screen_side_is_independent_of_driving_side(rhd, physical_yaw, side, direction):
  # DM mirrors yaw for RHD. A physical right head turn must still wake a
  # right-mounted screen, regardless of the seat the driver occupies.
  dm = monitoring_state(physical_yaw, rhd=rhd)
  assert screen_yaw(dm, side) == pytest.approx(physical_yaw * direction, abs=1e-5)


@pytest.mark.parametrize('rhd', [False, True])
def test_learned_neutral_is_subtracted_before_undoing_rhd(rhd):
  dm = monitoring_state(20, rhd=rhd, neutral=0.2, calibrated=True)
  assert screen_yaw(dm, 0) == pytest.approx(20, abs=1e-5)


@pytest.mark.parametrize('learned,bounded', [(-1, -0.0246), (1, 0.289)])
def test_calibrated_neutral_uses_driver_monitoring_bounds(learned, bounded):
  dm = monitoring_state(calibrated=True, neutral=learned)
  dm.visionPolicyState.pose.yaw = bounded + math.radians(20)
  assert screen_yaw(dm, 0) == pytest.approx(20, abs=1e-5)


def test_unfinished_calibration_uses_natural_pose_instead_of_partial_offset():
  dm = monitoring_state(20)
  dm.visionPolicyState.pose.yawCalib.offset = -1
  assert screen_yaw(dm, 0) == pytest.approx(20, abs=1e-5)


@pytest.mark.parametrize('condition', ['missing', 'wheeltouch', 'no_face', 'uncertain', 'negative_uncertainty',
                                       'nan_yaw', 'infinite_uncertainty', 'nan_neutral'])
def test_unreliable_vision_does_not_request_dimming(condition):
  dm = monitoring_state(calibrated=True)
  if condition == 'missing':
    dm = None
  elif condition == 'wheeltouch':
    dm.activePolicy = 'wheeltouch'
  elif condition == 'no_face':
    dm.visionPolicyState.faceDetected = False
  elif condition == 'uncertain':
    dm.visionPolicyState.pose.uncertainty = 0.3
  elif condition == 'negative_uncertainty':
    dm.visionPolicyState.pose.uncertainty = -0.1
  elif condition == 'nan_yaw':
    dm.visionPolicyState.pose.yaw = float('nan')
  elif condition == 'infinite_uncertainty':
    dm.visionPolicyState.pose.uncertainty = float('inf')
  elif condition == 'nan_neutral':
    dm.visionPolicyState.pose.yawCalib.offset = float('nan')
  assert screen_yaw(dm, 0) is None


def test_continuous_look_away_delay_and_immediate_screenward_glance():
  dimmer = ScreenGazeDimmer()
  preferences = GazeDimmingPreferences(enabled=True)
  assert dimmer.brightness(80, 100, preferences, 0) == 80
  assert dimmer.brightness(80, 102.9, preferences, 0) == 80
  assert dimmer.brightness(80, 103, preferences, 0) == 16
  assert dimmer.brightness(80, 103.1, preferences, 20) == 80
  assert dimmer.brightness(80, 104, preferences, 0) == 80
  assert dimmer.brightness(80, 106.9, preferences, 0) == 80
  assert dimmer.brightness(80, 107, preferences, 0) == 16


def test_threshold_hysteresis_does_not_oscillate_at_edge_of_screen():
  dimmer = ScreenGazeDimmer()
  preferences = GazeDimmingPreferences(enabled=True, yaw_threshold=20, delay=1)
  assert dimmer.brightness(80, 0, preferences, 19) == 80
  assert dimmer.brightness(80, 1, preferences, 19) == 16
  assert dimmer.brightness(80, 2, preferences, 20) == 80
  # Once looking toward the screen, jitter within three degrees stays bright.
  assert dimmer.brightness(80, 3, preferences, 17) == 80
  assert dimmer.brightness(80, 5, preferences, 17) == 80
  assert dimmer.brightness(80, 6, preferences, 16.9) == 80
  assert dimmer.brightness(80, 7, preferences, 16.9) == 16


@pytest.mark.parametrize('reset_reason', ['disabled', 'suppressed', 'missing', 'nan'])
def test_fail_bright_and_restart_entire_delay_after_suppression(reset_reason):
  dimmer = ScreenGazeDimmer()
  preferences = GazeDimmingPreferences(enabled=True, delay=2)
  assert dimmer.brightness(50, 0, preferences, 0) == 50
  assert dimmer.brightness(50, 2, preferences, 0) == 10
  selected = replace(preferences, enabled=False) if reset_reason == 'disabled' else preferences
  yaw = None if reset_reason == 'missing' else float('nan') if reset_reason == 'nan' else 0
  assert dimmer.brightness(50, 3, selected, yaw, suppressed=reset_reason == 'suppressed') == 50
  assert dimmer.brightness(50, 4, preferences, 0) == 50
  assert dimmer.brightness(50, 5.9, preferences, 0) == 50
  assert dimmer.brightness(50, 6, preferences, 0) == 10


@pytest.mark.parametrize('normal,percent,expected', [(0, 20, 0), (1, 20, 1), (4, 20, 4), (10, 20, 5),
                                                  (80, 20, 16), (80, 100, 80), (100, 5, 5)])
def test_dim_level_respects_configured_fraction_and_never_brightens_dark_display(normal, percent, expected):
  dimmer = ScreenGazeDimmer()
  preferences = GazeDimmingPreferences(enabled=True, delay=1, percent=percent)
  dimmer.brightness(normal, 0, preferences, 0)
  assert dimmer.brightness(normal, 1, preferences, 0) == expected
