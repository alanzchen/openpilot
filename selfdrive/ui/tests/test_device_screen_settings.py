from types import SimpleNamespace

import math
import pytest
from cereal import log

from openpilot.selfdrive.ui import ui_state as ui_state_module


class FakeParams:
  def __init__(self, **values):
    self.values = values

  def get(self, key):
    return self.values.get(key)

  def get_bool(self, key, **_kwargs):
    return bool(self.values.get(key, False))

  def get_int(self, key, **_kwargs):
    return int(self.values.get(key, 0))


class PassthroughFilter:
  def update(self, value):
    return value


def make_device(monkeypatch, **overrides):
  values = {
    "ScreenManagement": True,
    "ScreenBrightness": 35,
    "ScreenBrightnessOnroad": 45,
    "ScreenTimeout": 30,
    "ScreenTimeoutOnroad": 10,
    "StandbyMode": False,
  }
  values.update(overrides)
  state = SimpleNamespace(
    ui_params=FakeParams(**values),
    params_memory=FakeParams(),
    status=ui_state_module.UIStatus.DISENGAGED,
    started=False,
    ignition=False,
    light_sensor=-1.0,
    sm={},
  )
  monkeypatch.setattr(ui_state_module, "ui_state", state)
  monkeypatch.setattr(ui_state_module.gui_app, "big_ui", lambda: False)
  monkeypatch.setattr(ui_state_module.gui_app, "_mouse_events", [])
  device = ui_state_module.Device()
  device._brightness_filter = PassthroughFilter()
  return device, state


def test_manual_brightness_applies_to_current_device_state(monkeypatch):
  device, state = make_device(monkeypatch)

  assert device._calculate_brightness() == 35

  state.started = True
  assert device._calculate_brightness() == 45


def test_auto_brightness_preserves_existing_behavior(monkeypatch):
  device, state = make_device(monkeypatch, ScreenBrightness=101, ScreenBrightnessOnroad=101)

  assert device._calculate_brightness() == ui_state_module.BACKLIGHT_OFFROAD

  state.started = True
  state.light_sensor = -1.0
  assert device._calculate_brightness() == ui_state_module.BACKLIGHT_OFFROAD


def test_screen_management_off_ignores_custom_values(monkeypatch):
  device, state = make_device(monkeypatch, ScreenManagement=False, ScreenBrightness=10, ScreenBrightnessOnroad=20,
                              StandbyMode=True)

  assert device._calculate_brightness() == ui_state_module.BACKLIGHT_OFFROAD

  state.started = True
  device._interaction_time = 0
  assert device._calculate_brightness() == ui_state_module.BACKLIGHT_OFFROAD
  assert device.interactive_timeout == 30


def test_screen_settings_refresh_after_external_param_change(monkeypatch):
  now = 100.0
  monkeypatch.setattr(ui_state_module.time, "monotonic", lambda: now)
  device, state = make_device(monkeypatch)
  state.started = True
  state.ignition = True
  device._ignition = True
  device._interaction_time = now + 10

  state.ui_params.values["ScreenBrightnessOnroad"] = 72
  state.ui_params.values["ScreenTimeoutOnroad"] = 25
  now += device.SCREEN_SETTINGS_REFRESH_INTERVAL
  device._refresh_screen_settings()

  assert device._calculate_brightness() == 72
  assert device.interactive_timeout == 25
  assert device._interaction_time == now + 25


def test_standby_blanks_after_timeout_and_touch_wakes(monkeypatch):
  now = 100.0
  monkeypatch.setattr(ui_state_module.time, "monotonic", lambda: now)
  device, state = make_device(monkeypatch, StandbyMode=True)
  state.started = True
  state.ignition = True
  device._ignition = True
  device._interaction_time = now - 1

  assert device._calculate_brightness() == 0

  monkeypatch.setattr(ui_state_module.gui_app, "_mouse_events", [SimpleNamespace(left_down=True)])
  device._update_wakefulness()

  assert device._interaction_time == now + 10
  assert device._calculate_brightness() == 45


def test_standby_powers_down_onroad_and_touch_wakes(monkeypatch):
  now = 100.0
  monkeypatch.setattr(ui_state_module.time, "monotonic", lambda: now)
  monkeypatch.setattr(ui_state_module, "PC", False)
  display_power = []
  monkeypatch.setattr(ui_state_module.HARDWARE, "set_display_power", display_power.append)
  device, state = make_device(monkeypatch, StandbyMode=True)
  state.started = True
  state.ignition = True
  device._ignition = True
  device._interaction_time = now - 1

  device._update_wakefulness()

  assert display_power == [False]
  assert not device.awake
  assert device._calculate_brightness() == 0

  monkeypatch.setattr(ui_state_module.gui_app, "_mouse_events", [SimpleNamespace(left_down=True)])
  device._update_wakefulness()

  assert display_power == [False, True]
  assert device.awake
  assert device._calculate_brightness() == 45


def test_hide_ui_blanks_after_timeout_and_touch_wakes(monkeypatch):
  now = 100.0
  monkeypatch.setattr(ui_state_module.time, "monotonic", lambda: now)
  device, state = make_device(monkeypatch, ScreenBrightnessOnroad=0)
  state.started = True
  state.ignition = True
  device._ignition = True
  device._interaction_time = now - 1

  assert device._calculate_brightness() == 0

  monkeypatch.setattr(ui_state_module.gui_app, "_mouse_events", [SimpleNamespace(left_down=True)])
  device._update_wakefulness()

  assert device._interaction_time == now + 10
  assert device._calculate_brightness() == 5


def test_standby_wakes_for_visible_alert(monkeypatch):
  now = 100.0
  monkeypatch.setattr(ui_state_module.time, "monotonic", lambda: now)
  device, state = make_device(monkeypatch, StandbyMode=True)
  state.started = True
  state.ignition = True
  device._ignition = True
  device._interaction_time = now - 1
  device._active_standby_alerts = lambda: {"StandbyWakeInfoAlert"}

  device._update_wakefulness()

  assert device._interaction_time == now + 10
  assert device._calculate_brightness() == 45


class GazeSubMaster(dict):
  def __init__(self):
    dm = log.DriverMonitoringState.new_message()
    dm.activePolicy = 'vision'
    dm.visionPolicyState.faceDetected = True
    dm.visionPolicyState.pose.yaw = 0.075
    dm.visionPolicyState.pose.uncertainty = 0.1
    super().__init__(driverMonitoringState=dm, selfdriveState=log.SelfdriveState.new_message())
    self.alive = {'driverMonitoringState': True}
    self.valid = {'driverMonitoringState': True}
    self.recv_frame = {'driverMonitoringState': 11}
    self.logMonoTime = {'driverMonitoringState': 0}


def make_gaze_device(monkeypatch, **overrides):
  clock = [100.0]
  monkeypatch.setattr(ui_state_module.time, 'monotonic', lambda: clock[0])
  values = dict(ScreenGazeDimming=True, ScreenGazeDimPercent=20, ScreenGazeDimDelay=3,
                ScreenGazeScreenSide=0, ScreenGazeYawThreshold=15)
  values.update(overrides)
  device, state = make_device(monkeypatch, **values)
  state.started = state.ignition = True
  state.started_frame = 10
  state.sm = GazeSubMaster()
  device._ignition = device._screen_off_started = True
  device._interaction_time = 110

  def advance(seconds=0, *, new_packet=True):
    clock[0] += seconds
    if new_packet:
      state.sm.logMonoTime['driverMonitoringState'] = int(clock[0] * 1e9)
      state.sm.recv_frame['driverMonitoringState'] += 1
    return clock[0]

  advance()
  return device, state, advance


def test_gaze_dimming_uses_live_monitoring_and_restores_brightness_immediately(monkeypatch):
  device, state, advance = make_gaze_device(monkeypatch)
  assert device._calculate_brightness() == 45
  advance(3)
  assert device._calculate_brightness() == 9
  state.sm['driverMonitoringState'].visionPolicyState.pose.yaw = 0.075 + math.radians(25)
  advance()
  assert device._calculate_brightness() == 45


@pytest.mark.parametrize('side,rhd', [(0, False), (0, True), (1, False), (1, True)])
def test_configured_screen_side_and_rhd_are_used_by_device(monkeypatch, side, rhd):
  device, state, advance = make_gaze_device(monkeypatch, ScreenGazeScreenSide=side)
  dm = state.sm['driverMonitoringState']
  dm.isRHD = rhd
  device._calculate_brightness()
  advance(3)
  assert device._calculate_brightness() == 9
  screenward = math.radians(25) * (-1 if side else 1) * (-1 if rhd else 1)
  dm.visionPolicyState.pose.yaw = 0.075 - screenward
  advance()
  assert device._calculate_brightness() == 9
  dm.visionPolicyState.pose.yaw = 0.075 + screenward
  advance()
  assert device._calculate_brightness() == 45


@pytest.mark.parametrize('overrides', [{'ScreenGazeDimming': False}, {'ScreenManagement': False}])
def test_gaze_dimming_is_opt_in_and_requires_screen_management(monkeypatch, overrides):
  device, _, advance = make_gaze_device(monkeypatch, **overrides)
  normal = 45 if overrides.get('ScreenManagement', True) else ui_state_module.BACKLIGHT_OFFROAD
  assert device._calculate_brightness() == normal
  advance(30)
  assert device._calculate_brightness() == normal


def test_offroad_resets_gaze_delay_and_keeps_parked_brightness(monkeypatch):
  device, state, advance = make_gaze_device(monkeypatch)
  device._calculate_brightness()
  advance(3)
  assert device._calculate_brightness() == 9
  state.started = False
  assert device._calculate_brightness() == 35
  advance(10)
  state.started = True
  assert device._calculate_brightness() == 45
  advance(3)
  assert device._calculate_brightness() == 9


@pytest.mark.parametrize('failure', ['dead', 'invalid', 'stale', 'future', 'before_onroad', 'dm_alert'])
def test_unusable_monitoring_restores_brightness_and_restarts_delay(monkeypatch, failure):
  device, state, advance = make_gaze_device(monkeypatch)
  device._calculate_brightness()
  now = advance(3)
  assert device._calculate_brightness() == 9
  sm = state.sm
  if failure == 'dead':
    sm.alive['driverMonitoringState'] = False
  elif failure == 'invalid':
    sm.valid['driverMonitoringState'] = False
  elif failure == 'stale':
    sm.logMonoTime['driverMonitoringState'] = int((now - 0.5) * 1e9)
  elif failure == 'future':
    sm.logMonoTime['driverMonitoringState'] = int((now + 1) * 1e9)
  elif failure == 'before_onroad':
    sm.recv_frame['driverMonitoringState'] = state.started_frame
  elif failure == 'dm_alert':
    sm['driverMonitoringState'].alertLevel = 'one'
    advance()
  assert device._calculate_brightness() == 45
  sm.alive['driverMonitoringState'] = sm.valid['driverMonitoringState'] = True
  sm.recv_frame['driverMonitoringState'] = state.started_frame + 1
  sm['driverMonitoringState'].alertLevel = 'none'
  advance(1)
  assert device._calculate_brightness() == 45
  advance(2.9)
  assert device._calculate_brightness() == 45
  advance(0.1)
  assert device._calculate_brightness() == 9


def test_gaze_dimming_touch_restore_works_with_standby_disabled(monkeypatch):
  device, _, advance = make_gaze_device(monkeypatch, StandbyMode=False)
  device._calculate_brightness()
  advance(3)
  assert device._calculate_brightness() == 9
  monkeypatch.setattr(ui_state_module.gui_app, '_mouse_events', [SimpleNamespace(left_down=True)])
  device._update_wakefulness()
  assert device._calculate_brightness() == 45
  monkeypatch.setattr(ui_state_module.gui_app, '_mouse_events', [])
  advance(2.9)
  assert device._calculate_brightness() == 45
  advance(0.1)
  assert device._calculate_brightness() == 9


@pytest.mark.parametrize('status,size', [('normal', 'small'), ('userPrompt', 'mid'), ('critical', 'full')])
def test_all_alerts_suspend_gaze_dimming_independent_of_standby_wake_choices(monkeypatch, status, size):
  device, state, advance = make_gaze_device(monkeypatch, StandbyWakeInfoAlert=False, StandbyWakeWarningAlert=False,
                                          StandbyWakeCriticalAlert=False)
  assert not device._wake_keys & {'StandbyWakeInfoAlert', 'StandbyWakeWarningAlert', 'StandbyWakeCriticalAlert'}
  device._calculate_brightness()
  advance(3)
  assert device._calculate_brightness() == 9
  alert = state.sm['selfdriveState']
  alert.alertStatus, alert.alertSize = status, size
  assert device._calculate_brightness() == 45
  advance(10)
  assert device._calculate_brightness() == 45
  alert.alertStatus, alert.alertSize = 'normal', 'none'
  assert device._calculate_brightness() == 45
  advance(3)
  assert device._calculate_brightness() == 9


@pytest.mark.parametrize('overrides,expected', [
  ({'ScreenBrightnessOnroad': 101, 'ScreenBrightnessOnroadOffset': -20}, 16),
  ({'ScreenBrightnessOnroad': 10}, 5),
  ({'ScreenBrightnessOnroad': 2}, 2),
  ({'ScreenGazeDimPercent': 50, 'ScreenBrightnessOnroad': 80}, 40),
])
def test_gaze_dim_fraction_applies_after_auto_or_manual_brightness(monkeypatch, overrides, expected):
  device, state, advance = make_gaze_device(monkeypatch, **overrides)
  state.light_sensor = 100
  # Stop the temporary visibility minimum from changing intentional manual 2%.
  device._interaction_time = 99
  device._calculate_brightness()
  advance(3)
  assert device._calculate_brightness() == expected


@pytest.mark.parametrize('blank_mode', ['standby', 'manual_zero', 'screen_off', 'asleep'])
def test_explicit_blanking_has_precedence_over_gaze_dimming(monkeypatch, blank_mode):
  values = {'StandbyMode': blank_mode == 'standby', 'ScreenBrightnessOnroad': 0 if blank_mode == 'manual_zero' else 45}
  device, _, advance = make_gaze_device(monkeypatch, **values)
  device._interaction_time = 99
  device._screen_off = blank_mode == 'screen_off'
  device._awake = blank_mode != 'asleep'
  assert device._calculate_brightness() == 0
  advance(3)
  assert device._calculate_brightness() == 0


def test_settings_saved_externally_refresh_and_restart_gaze_delay(monkeypatch):
  device, state, advance = make_gaze_device(monkeypatch)
  device._calculate_brightness()
  advance(3)
  assert device._calculate_brightness() == 9
  state.ui_params.values['ScreenGazeDimPercent'] = 50
  device._refresh_screen_settings()
  assert device._calculate_brightness() == 45
  advance(3)
  assert device._calculate_brightness() == 22
  state.ui_params.values['ScreenGazeDimming'] = False
  device._refresh_screen_settings()
  assert device._calculate_brightness() == 45


@pytest.mark.parametrize('mode', ['disabled', 'offroad', 'blank'])
def test_inactive_gaze_path_skips_monitoring_and_extra_alert_reads(monkeypatch, mode):
  device, state, _ = make_gaze_device(monkeypatch, ScreenGazeDimming=mode != 'disabled')
  if mode == 'offroad':
    state.started = False
  if mode == 'blank':
    device._screen_brightness_onroad = 0
    device._interaction_time = 99

  def unexpected_read(*_args, **_kwargs):
    raise AssertionError('Inactive gaze dimming should not read monitoring or alerts')

  monkeypatch.setattr(device, '_screen_gaze_yaw', unexpected_read)
  monkeypatch.setattr(device, '_active_standby_alerts', unexpected_read)
  assert device._calculate_brightness() == (0 if mode == 'blank' else 35 if mode == 'offroad' else 45)


def test_gaze_pose_decodes_once_per_packet_while_deadline_and_staleness_tick_each_frame(monkeypatch):
  device, _, advance = make_gaze_device(monkeypatch)
  conversions = []
  original = ui_state_module.screen_yaw

  def convert(dm, side):
    conversions.append(side)
    return original(dm, side)

  monkeypatch.setattr(ui_state_module, 'screen_yaw', convert)
  assert device._calculate_brightness() == 45
  for _ in range(10):
    assert device._calculate_brightness() == 45
  assert len(conversions) == 1

  advance(2.8)
  assert device._calculate_brightness() == 45
  advance(0.21, new_packet=False)
  assert device._calculate_brightness() == 9
  assert len(conversions) == 2

  # A cached pose must stop dimming as soon as its unchanged source expires.
  advance(0.3, new_packet=False)
  assert device._calculate_brightness() == 45
  assert len(conversions) == 2
  advance()
  assert device._calculate_brightness() == 45
  advance(3)
  assert device._calculate_brightness() == 9
  assert len(conversions) == 4


@pytest.mark.parametrize('flag', ['alive', 'valid'])
def test_cached_gaze_pose_rechecks_message_health_without_new_packet(monkeypatch, flag):
  device, state, advance = make_gaze_device(monkeypatch)
  assert device._screen_gaze_yaw(ui_state_module.time.monotonic()) == pytest.approx(0, abs=1e-5)
  getattr(state.sm, flag)['driverMonitoringState'] = False
  advance(0.1, new_packet=False)
  assert device._screen_gaze_yaw(ui_state_module.time.monotonic()) is None
  getattr(state.sm, flag)['driverMonitoringState'] = True
  advance(0.1, new_packet=False)
  assert device._screen_gaze_yaw(ui_state_module.time.monotonic()) == pytest.approx(0, abs=1e-5)


def test_non_dm_alert_packet_restores_cached_gaze_brightness_immediately(monkeypatch):
  device, state, advance = make_gaze_device(monkeypatch)
  device._calculate_brightness()
  advance(3)
  assert device._calculate_brightness() == 9
  frame = state.sm.recv_frame['driverMonitoringState']
  state.sm['selfdriveState'].alertSize = 'small'
  advance(0.1, new_packet=False)
  assert state.sm.recv_frame['driverMonitoringState'] == frame
  assert device._calculate_brightness() == 45


def test_screen_side_setting_invalidates_cached_pose_without_new_dm_packet(monkeypatch):
  device, state, advance = make_gaze_device(monkeypatch)
  state.sm['driverMonitoringState'].visionPolicyState.pose.yaw = 0.075 - math.radians(25)
  device._calculate_brightness()
  advance(3)
  assert device._calculate_brightness() == 9
  frame = state.sm.recv_frame['driverMonitoringState']
  state.ui_params.values['ScreenGazeScreenSide'] = 1
  device._refresh_screen_settings(force=True)
  assert state.sm.recv_frame['driverMonitoringState'] == frame
  assert device._screen_gaze_yaw(ui_state_module.time.monotonic()) == pytest.approx(25, abs=1e-5)
  assert device._calculate_brightness() == 45


@pytest.mark.parametrize('enabled', [False, True])
def test_device_update_reuses_wakefulness_alert_snapshot_for_brightness(monkeypatch, enabled):
  device, _, _ = make_gaze_device(monkeypatch, ScreenGazeDimming=enabled)
  scans = []
  calculated_alerts = []
  calculate = device._calculate_brightness

  def active_alerts():
    scans.append(True)
    return {'StandbyWakeInfoAlert'}

  def update_brightness(active_alerts=None):
    calculated_alerts.append(active_alerts)
    assert calculate(active_alerts) == 45

  monkeypatch.setattr(device, '_active_standby_alerts', active_alerts)
  monkeypatch.setattr(device, '_update_brightness', update_brightness)
  device.update()
  assert len(scans) == 1
  assert calculated_alerts == [{'StandbyWakeInfoAlert'}]
