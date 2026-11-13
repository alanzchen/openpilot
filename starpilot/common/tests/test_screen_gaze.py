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


def follow_preferences(**overrides):
  return replace(GazeDimmingPreferences(enabled=True, mode=1), **overrides)


def settle_follow(dimmer, preferences, yaw=0, normal=100, *, fps=60, start=0):
  values = [dimmer.brightness(normal, start, preferences, yaw)]
  for frame in range(1, fps + 1):
    values.append(dimmer.brightness(normal, start + frame / fps, preferences, yaw))
  return values


@pytest.mark.parametrize('yaw,expected', [(-60, 20), (0, 20), (3, 28), (7.5, 60), (12, 92), (15, 100), (60, 100)])
def test_follow_head_direction_has_continuous_angle_curve(yaw, expected):
  dimmer = ScreenGazeDimmer()
  values = settle_follow(dimmer, follow_preferences(yaw_threshold=15), yaw)
  assert values[0] == 100
  assert values[-1] == expected


def test_follow_head_angle_brightness_is_monotonic_across_intermediate_angles():
  preferences = follow_preferences(yaw_threshold=20)
  levels = [settle_follow(ScreenGazeDimmer(), preferences, yaw)[-1] for yaw in range(-5, 26)]
  assert levels == sorted(levels)
  assert len(set(levels)) > 15
  assert levels[0] == 20
  assert levels[-1] == 100


def test_follow_mode_ignores_binary_delay_and_completes_most_fade_in_quarter_second():
  dimmer = ScreenGazeDimmer()
  preferences = follow_preferences(delay=30)
  levels = [dimmer.brightness(100, 100 + frame / 60, preferences, 0) for frame in range(31)]
  assert levels[0] == 100
  assert levels[1] < 100
  assert levels[13] <= 28  # At least 90% of the 80-point fade by ~217 ms.
  assert levels[15] <= 24  # At least 95% by 250 ms.
  assert levels[21] <= 21  # Almost settled by 350 ms, without overshooting.
  assert levels == sorted(levels, reverse=True)
  assert all(20 <= level <= 100 for level in levels)


def test_rapid_head_turns_ramp_in_both_directions_without_two_level_jumps():
  dimmer = ScreenGazeDimmer()
  preferences = follow_preferences()
  assert settle_follow(dimmer, preferences)[-1] == 20
  rising = [20] + [dimmer.brightness(100, 1 + frame / 60, preferences, 30) for frame in range(1, 28)]
  assert rising == sorted(rising)
  assert 20 < rising[1] < 100
  assert rising[-1] == 100
  assert all(0 <= after - before <= 7 for before, after in zip(rising, rising[1:], strict=False))
  falling = [100] + [dimmer.brightness(100, 1 + 27 / 60 + frame / 60, preferences, -30) for frame in range(1, 28)]
  assert falling == sorted(falling, reverse=True)
  assert 20 < falling[1] < 100
  assert falling[-1] == 20
  assert all(0 <= before - after <= 7 for before, after in zip(falling, falling[1:], strict=False))


@pytest.mark.parametrize('fps', [20, 30, 60, 120])
def test_follow_slew_tracks_elapsed_time_across_normal_frame_cadences(fps):
  dimmer = ScreenGazeDimmer()
  preferences = follow_preferences()
  levels = settle_follow(dimmer, preferences, fps=fps)
  assert levels[0] == 100
  assert levels[round(fps / 10)] == 60
  for threshold, deadline in [(28, 0.22), (24, 0.25), (21, 0.35)]:
    reached = next(frame / fps for frame, level in enumerate(levels) if level <= threshold)
    assert reached <= deadline + 1 / fps
  assert levels[-1] == 20


def test_follow_mode_attenuates_steady_dm_jitter_without_freezing_real_changes():
  dimmer = ScreenGazeDimmer()
  preferences = follow_preferences(yaw_threshold=15)
  assert settle_follow(dimmer, preferences, yaw=7.5)[-1] == 60
  levels = []
  for frame in range(1, 241):
    # A one-degree error on alternating 20 Hz packets produces a 16-point
    # unfiltered target swing near the curve's most sensitive midpoint.
    yaw = 6.5 if ((frame - 1) // 3) % 2 == 0 else 8.5
    levels.append(dimmer.brightness(100, 1 + frame / 60, preferences, yaw))
  steady = levels[-60:]
  assert 2 <= max(steady) - min(steady) <= 8
  assert sum(steady) / len(steady) == pytest.approx(60, abs=1)
  assert all(52 <= level <= 68 for level in steady)
  assert all(abs(after - before) <= 4 for before, after in zip(steady, steady[1:], strict=False))


@pytest.mark.parametrize('reset_reason', ['disabled', 'suppressed', 'missing', 'nan', 'infinite', 'explicit'])
def test_follow_mode_restores_normal_immediately_and_restarts_smoothly_after_reset(reset_reason):
  dimmer = ScreenGazeDimmer()
  preferences = follow_preferences()
  assert settle_follow(dimmer, preferences)[-1] == 20
  selected = replace(preferences, enabled=False) if reset_reason == 'disabled' else preferences
  yaw = {'missing': None, 'nan': float('nan'), 'infinite': float('inf')}.get(reset_reason, 0)
  if reset_reason == 'explicit':
    dimmer.reset()
  assert dimmer.brightness(100, 1.01, selected, yaw, suppressed=reset_reason == 'suppressed') == 100
  assert dimmer.brightness(100, 1.02, preferences, 0) == (96 if reset_reason == 'explicit' else 100)
  next_level = dimmer.brightness(100, 1.03, preferences, 0)
  assert 90 <= next_level < 100


@pytest.mark.parametrize('now,next_expected', [(0.5, 100), (2, 80), (float('nan'), 100), (float('inf'), 100)])
def test_follow_mode_resets_bright_after_bad_clock_or_long_gap(now, next_expected):
  dimmer = ScreenGazeDimmer()
  preferences = follow_preferences()
  assert settle_follow(dimmer, preferences)[-1] == 20
  assert dimmer.brightness(100, now, preferences, 0) == 100
  assert dimmer.brightness(100, 2.1, preferences, 0) == next_expected


def test_follow_mode_caps_a_delayed_frames_visible_step_and_repeated_time_is_stable():
  dimmer = ScreenGazeDimmer()
  preferences = follow_preferences()
  assert dimmer.brightness(100, 0, preferences, 0) == 100
  assert dimmer.brightness(100, 0.4, preferences, 0) == 80
  assert dimmer.brightness(100, 0.4, preferences, 0) == 80
  assert dimmer.brightness(100, 0.45, preferences, 0) == 60


def test_mode_changes_restart_bright_without_inheriting_old_mode_timers():
  dimmer = ScreenGazeDimmer()
  follow = follow_preferences()
  delayed = replace(follow, mode=0, delay=3)
  assert settle_follow(dimmer, follow)[-1] == 20
  assert dimmer.brightness(100, 1.1, delayed, 0) == 100
  assert dimmer.brightness(100, 3, delayed, 0) == 100
  assert dimmer.brightness(100, 4.11, delayed, 0) == 20
  assert dimmer.brightness(100, 4.2, follow, 0) == 100
  assert dimmer.brightness(100, 4.25, follow, 0) == 80


@pytest.mark.parametrize('normal,percent,expected', [(0, 20, 0), (1, 20, 1), (4, 20, 4), (10, 20, 5),
                                                  (80, 20, 16), (80, 100, 80), (100, 5, 5)])
def test_follow_mode_respects_floor_and_never_brightens_an_intentionally_dark_display(normal, percent, expected):
  values = settle_follow(ScreenGazeDimmer(), follow_preferences(percent=percent), normal=normal)
  assert all(0 <= value <= normal for value in values)
  assert values[-1] == expected
  if percent == 100:
    assert set(values) == {normal}
