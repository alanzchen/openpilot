"""Exercise the native settings page with real rows, cache and screen validator."""
import ast
import importlib
import os
from pathlib import Path
from types import SimpleNamespace

import pytest

os.environ.setdefault('SP_HEADLESS_TEST', '1')

from openpilot.selfdrive.ui.lib.ui_param_cache import UIParamCache
from openpilot.common.params import UnknownKeyName
from openpilot.starpilot.common.screen_settings import SCREEN_GAZE_RANGES, write_screen_setting
from openpilot.system.ui.widgets import DialogResult


class StoredParams:
  def __init__(self, directory, values):
    self.directory = directory
    self.values = dict(values)
    self.writes = []
    self.fail_key = None
    self.silent_key = None
    self.unknown_key = None

  def get_param_path(self):
    return str(self.directory / 'd')

  def get(self, key, **_kwargs):
    value = self.values.get(key)
    if value is None or isinstance(value, bytes):
      return value
    return str(int(value) if isinstance(value, bool) else value).encode()

  def get_bool(self, key, **_kwargs):
    return self.get(key) in (b'1', b'true')

  def get_int(self, key, **_kwargs):
    return int(self.get(key) or 0)

  def put_int(self, key, value):
    if key == self.unknown_key:
      raise UnknownKeyName(key)
    if key == self.fail_key:
      raise OSError('Storage unavailable')
    self.writes.append((key, value))
    if key != self.silent_key:
      self.values[key] = value

  def put_bool(self, key, value):
    self.put_int(key, value)

  def remove(self, key):
    self.values.pop(key, None)


class SliderDialog:
  def __init__(self, title, min_val, max_val, step, current_val, on_close, **kwargs):
    self.title, self.minimum, self.maximum, self.step = title, min_val, max_val, step
    self.value, self.callback, self.kwargs = current_val, on_close, kwargs


class SideDialog:
  def __init__(self, title, options, current='', *, callback=None, **_kwargs):
    self.title, self.options, self.selection = title, options, current
    self.callback = callback


@pytest.fixture
def native_page(monkeypatch, tmp_path):
  module = importlib.import_module('openpilot.selfdrive.ui.layouts.settings.starpilot.screen_gaze_settings')
  storage = StoredParams(tmp_path, {'ScreenManagement': True, 'ScreenGazeDimming': True})
  cache = UIParamCache(storage, ttl=60)
  pushed, errors, interactions = [], [], []
  monkeypatch.setattr(module.gui_app, 'push_widget', pushed.append)
  monkeypatch.setattr(module, 'alert_dialog', errors.append)
  monkeypatch.setattr(module, 'AetherSliderDialog', SliderDialog)
  monkeypatch.setattr(module, 'MultiOptionDialog', SideDialog)
  monkeypatch.setattr(module.device, 'reset_interactive_timeout', lambda: interactions.append(True))
  page = module.ScreenGazeSettingsView(SimpleNamespace(_params=cache))
  return SimpleNamespace(page=page, storage=storage, cache=cache, pushed=pushed, errors=errors, interactions=interactions)


def visible_rows(page):
  return {row.id: row for section in page._active_sections() for row in page._visible_rows(section)}


def test_defaults_are_shared_with_runtime_and_do_not_write_on_open(native_page):
  page, storage = native_page.page, native_page.storage
  assert page._parent_toggle.get_state()
  for key, (default, _minimum, _maximum) in SCREEN_GAZE_RANGES.items():
    assert page._value(key) == default
  assert storage.writes == []


def test_disabled_feature_hides_children_and_toggle_preserves_configuration(native_page):
  page, storage = native_page.page, native_page.storage
  storage.values.update(ScreenGazeDimPercent=35, ScreenGazeDimDelay=8, ScreenGazeScreenSide=1, ScreenGazeYawThreshold=25)
  page._set_enabled(False)
  assert not page._parent_toggle.get_state()
  assert not visible_rows(page)
  page._set_enabled(True)
  assert page._parent_toggle.get_state()
  assert {key: page._value(key) for key in SCREEN_GAZE_RANGES} == {
    'ScreenGazeDimPercent': 35, 'ScreenGazeDimDelay': 8, 'ScreenGazeScreenSide': 1, 'ScreenGazeYawThreshold': 25,
    'ScreenGazeDimmingMode': 0,
  }


@pytest.mark.parametrize('mode', [0, 1])
def test_advanced_angle_is_local_and_hidden_until_expanded(native_page, mode):
  page, storage = native_page.page, native_page.storage
  storage.values['ScreenGazeDimmingMode'] = mode
  assert {'ScreenGazeDimmingMode', 'ScreenGazeDimPercent', 'ScreenGazeScreenSide'} <= visible_rows(page).keys()
  assert ('ScreenGazeDimDelay' in visible_rows(page)) == (mode == 0)
  assert 'ScreenGazeYawThreshold' not in visible_rows(page)
  page._toggle_advanced()
  assert 'ScreenGazeYawThreshold' in visible_rows(page)
  page._toggle_advanced()
  assert 'ScreenGazeYawThreshold' not in visible_rows(page)
  assert storage.writes == []


def test_screen_management_disabled_explains_recovery_and_prevents_ineffective_enable(native_page):
  page, storage = native_page.page, native_page.storage
  storage.values['ScreenManagement'] = False
  page.show_event()
  before = dict(storage.values)
  assert set(visible_rows(page)) == {'ScreenManagement'}
  page._set_enabled(True)
  assert storage.values == before
  assert native_page.errors
  assert not native_page.interactions
  page._activate_target('toggle:ScreenManagement')
  assert storage.values['ScreenManagement'] is True
  assert storage.values['ScreenGazeDimming'] is True
  assert 'ScreenGazeDimPercent' in visible_rows(page)


@pytest.mark.parametrize('key,value', [('ScreenGazeDimming', False), ('ScreenGazeDimPercent', 35),
                                     ('ScreenGazeDimDelay', 8), ('ScreenGazeScreenSide', 1), ('ScreenGazeYawThreshold', 25),
                                     ('ScreenGazeDimmingMode', 1)])
def test_native_save_persists_same_keys_as_galaxy_and_invalidates_cache(native_page, key, value):
  page, storage = native_page.page, native_page.storage
  native_page.cache.get(key)
  assert page._save(key, value)
  assert storage.values[key] == value
  assert native_page.cache.get(key) == storage.get(key)
  assert native_page.interactions == [True]
  assert not native_page.errors


@pytest.mark.parametrize('failure', ['exception', 'silent_write', 'invalid_value'])
def test_save_failure_keeps_stored_value_reports_error_and_does_not_wake(native_page, failure):
  page, storage = native_page.page, native_page.storage
  key = 'ScreenGazeDimPercent'
  storage.values[key] = 35
  if failure == 'exception':
    storage.fail_key = key
  elif failure == 'silent_write':
    storage.silent_key = key
  assert not page._save(key, 2 if failure == 'invalid_value' else 60)
  assert storage.values[key] == 35
  assert page._value(key) == 35
  assert native_page.errors
  assert not native_page.interactions


@pytest.mark.parametrize('key,value', [('ScreenGazeDimming', False), ('ScreenGazeDimmingMode', 1)])
def test_missing_native_registry_key_explains_required_update(native_page, key, value):
  page, storage = native_page.page, native_page.storage
  before = dict(storage.values)
  storage.unknown_key = key
  assert not page._save(key, value)
  assert storage.values == before
  assert storage.writes == []
  assert native_page.errors == [
    'Update and restart StarPilot to use these screen dimming settings.',
  ]
  assert not native_page.interactions


@pytest.mark.parametrize('key', ['ScreenGazeDimPercent', 'ScreenGazeDimDelay', 'ScreenGazeYawThreshold'])
def test_numeric_dialog_uses_validated_ranges_and_only_saves_confirmed_value(native_page, key):
  page, storage = native_page.page, native_page.storage
  page._show_numeric(key)
  dialog = native_page.pushed[-1]
  default, minimum, maximum = SCREEN_GAZE_RANGES[key]
  assert (dialog.minimum, dialog.maximum, dialog.step, dialog.value) == (minimum, maximum, 1, default)
  dialog.callback(DialogResult.CANCEL, maximum)
  assert storage.writes == []
  page._show_numeric(key)
  native_page.pushed[-1].callback(DialogResult.CONFIRM, maximum)
  assert storage.values[key] == maximum


def test_screen_side_selector_uses_physical_right_left_and_preserves_cancel(native_page):
  page, storage = native_page.page, native_page.storage
  page._show_screen_side()
  dialog = native_page.pushed[-1]
  assert dialog.options == ['Right', 'Left']
  assert dialog.selection == 'Right'
  dialog.selection = 'Left'
  dialog.callback(DialogResult.CANCEL)
  assert storage.writes == []
  page._show_screen_side()
  dialog = native_page.pushed[-1]
  dialog.selection = 'Left'
  dialog.callback(DialogResult.CONFIRM)
  assert storage.values['ScreenGazeScreenSide'] == 1


def test_dimming_style_defaults_to_after_delay_and_only_saves_confirmed_selection(native_page):
  page, storage = native_page.page, native_page.storage
  assert visible_rows(page)['ScreenGazeDimmingMode'].get_value() == 'After Delay'
  page._activate_target('value:ScreenGazeDimmingMode')
  dialog = native_page.pushed[-1]
  assert dialog.title == 'Dimming Style'
  assert dialog.options == ['After Delay', 'Follow Head Direction']
  assert dialog.selection == 'After Delay'
  dialog.selection = 'Follow Head Direction'
  dialog.callback(DialogResult.CANCEL)
  assert storage.writes == []
  assert 'ScreenGazeDimDelay' in visible_rows(page)

  page._show_dimming_style()
  dialog = native_page.pushed[-1]
  dialog.selection = 'Follow Head Direction'
  dialog.callback(DialogResult.CONFIRM)
  assert storage.values['ScreenGazeDimmingMode'] == 1
  assert visible_rows(page)['ScreenGazeDimmingMode'].get_value() == 'Follow Head Direction'
  assert 'ScreenGazeDimDelay' not in visible_rows(page)


def test_follow_head_direction_hides_delay_preserves_it_and_blocks_stale_click(native_page):
  page, storage = native_page.page, native_page.storage
  write_screen_setting(storage, 'ScreenGazeDimDelay', 12)
  assert 'ScreenGazeDimDelay' in visible_rows(page)
  write_screen_setting(storage, 'ScreenGazeDimmingMode', 1)
  native_page.cache.invalidate()
  assert 'ScreenGazeDimDelay' not in visible_rows(page)
  page._activate_target('value:ScreenGazeDimDelay')
  assert not native_page.pushed
  assert storage.values['ScreenGazeDimDelay'] == 12
  assert page._save('ScreenGazeDimmingMode', 0)
  assert visible_rows(page)['ScreenGazeDimDelay'].get_value() == '12 seconds'


@pytest.mark.parametrize('failure', ['exception', 'silent_write', 'invalid_value'])
def test_failed_style_save_preserves_existing_style_and_delay(native_page, failure):
  page, storage = native_page.page, native_page.storage
  storage.values.update(ScreenGazeDimmingMode=0, ScreenGazeDimDelay=12)
  if failure == 'exception':
    storage.fail_key = 'ScreenGazeDimmingMode'
  elif failure == 'silent_write':
    storage.silent_key = 'ScreenGazeDimmingMode'
  assert not page._save('ScreenGazeDimmingMode', 2 if failure == 'invalid_value' else 1)
  assert storage.values['ScreenGazeDimmingMode'] == 0
  assert storage.values['ScreenGazeDimDelay'] == 12
  assert 'ScreenGazeDimDelay' in visible_rows(page)
  assert native_page.errors == ['Screen setting could not be saved. Please try again.']
  assert not native_page.interactions


def test_external_galaxy_writes_are_refreshed_when_reopening(native_page):
  page, storage = native_page.page, native_page.storage
  assert page._value('ScreenGazeDimDelay') == 3
  assert page._parent_toggle.get_state()
  write_screen_setting(storage, 'ScreenGazeDimDelay', 12)
  write_screen_setting(storage, 'ScreenGazeScreenSide', 1)
  write_screen_setting(storage, 'ScreenGazeDimmingMode', 1)
  write_screen_setting(storage, 'ScreenGazeDimming', False)
  writes = list(storage.writes)
  page.show_event()
  assert page._value('ScreenGazeDimDelay') == 12
  assert page._value('ScreenGazeScreenSide') == 1
  assert page._value('ScreenGazeDimmingMode') == 1
  assert not page._parent_toggle.get_state()
  assert not visible_rows(page)
  assert storage.writes == writes


@pytest.mark.parametrize('disabled_key', ['ScreenManagement', 'ScreenGazeDimming'])
def test_stale_row_click_after_external_disable_does_not_open_dialog(native_page, disabled_key):
  page, storage = native_page.page, native_page.storage
  assert 'ScreenGazeDimPercent' in visible_rows(page)
  storage.values[disabled_key] = False
  native_page.cache.invalidate()
  page._activate_target('value:ScreenGazeDimPercent')
  page._activate_target('value:ScreenGazeScreenSide')
  page._activate_target('value:ScreenGazeDimmingMode')
  assert not native_page.pushed
  assert storage.writes == []


def test_hidden_advanced_control_cannot_open_from_stale_click(native_page):
  page = native_page.page
  page._activate_target('value:ScreenGazeYawThreshold')
  assert not native_page.pushed
  page._activate_target('value:advanced')
  page._activate_target('value:ScreenGazeYawThreshold')
  assert isinstance(native_page.pushed[-1], SliderDialog)


def test_stale_recovery_click_cannot_disable_master_after_galaxy_enabled_it(native_page):
  page, storage = native_page.page, native_page.storage
  storage.values['ScreenManagement'] = False
  page.show_event()
  assert 'ScreenManagement' in visible_rows(page)
  write_screen_setting(storage, 'ScreenManagement', True)
  native_page.cache.invalidate()
  page._activate_target('toggle:ScreenManagement')
  assert storage.values['ScreenManagement'] is True
  assert 'ScreenManagement' not in visible_rows(page)


def system_source():
  path = Path(__file__).resolve().parents[1] / 'layouts/settings/starpilot/system_settings.py'
  return path, ast.parse(path.read_text())


@pytest.fixture
def system_view(native_page):
  # Load the actual manager class without constructing the system page's
  # unrelated backup/storage/device services or opening a graphics context.
  from openpilot.selfdrive.ui.layouts.settings.starpilot import aethergrid

  path, source = system_source()
  manager = next(node for node in source.body if isinstance(node, ast.ClassDef) and node.name == 'SystemSettingsManagerView')
  namespace = {**vars(aethergrid), 'PANEL_STYLE': aethergrid.DEFAULT_PANEL_STYLE,
               'SYSTEM_PANEL_METRICS': aethergrid.AETHER_LIST_METRICS, 'tr': lambda value: value}
  module = ast.Module(body=[ast.ImportFrom(module='__future__', names=[ast.alias(name='annotations')], level=0), manager], type_ignores=[])
  exec(compile(ast.fix_missing_locations(module), str(path), 'exec'), namespace)
  view = object.__new__(namespace['SystemSettingsManagerView'])
  view._controller = SimpleNamespace(_params=native_page.cache)
  view._interactive_state = lambda *_args, **_kwargs: (False, False)
  view._display_slider_keys = ['ScreenBrightness', 'ScreenBrightnessOnroad', 'ScreenTimeout', 'ScreenTimeoutOnroad']
  view._power_slider_keys = ['DeviceShutdown', 'LowVoltageShutdown']

  class Adjustor:
    custom_row_height = None

    def measure_height(self, _width):
      return self.custom_row_height or 136

  view._adjustor_rows = {key: Adjustor() for key in view._display_slider_keys + view._power_slider_keys}
  view._connectivity_tile_grid = object()
  view.measure_page_grid_height = lambda *_args: 300
  view._render_page_grid = lambda *_args: None
  view._draw_two_column_tile_grid = lambda *_args, **_kwargs: None
  view._scroll_offset = 0
  rows, groups, sliders = [], [], []
  namespace['draw_settings_list_row'] = lambda rect, **kwargs: rows.append((rect, kwargs))
  namespace['draw_list_group_shell'] = lambda rect, **_kwargs: groups.append(rect)
  namespace['draw_group_header'] = lambda _x, y, _width, _title: y + aethergrid.GROUP_HEADER_TOTAL_HEIGHT

  def slider(rect, key, **_kwargs):
    height = view._adjustor_rows[key].measure_height(rect.width)
    sliders.append((key, rect.y + height))
    return rect.y + height

  view._draw_slider_row = slider
  return SimpleNamespace(view=view, rows=rows, groups=groups, sliders=sliders, aethergrid=aethergrid)


@pytest.mark.parametrize('management,gaze,summary', [(True, True, 'On'), (True, False, 'Off'), (False, True, 'Off')])
def test_system_display_entry_summary_uses_effective_enable_and_opens_route(native_page, system_view, management, gaze, summary):
  native_page.storage.values.update(ScreenManagement=management, ScreenGazeDimming=gaze)
  native_page.cache.invalidate()
  view = system_view.view
  opened = []
  view._controller.open_screen_gaze_settings = lambda: opened.append(True)
  rect = system_view.aethergrid.rl.Rectangle(0, 0, 600, 128)
  assert view._draw_screen_gaze_row(rect) == 128
  _, row = system_view.rows[-1]
  assert row['title'] == 'Dim When Looking Away'
  assert row['value'] == summary
  assert row['show_chevron']
  view._activate_target('screen_gaze')
  assert opened == [True]


def test_native_subpage_is_lazy_reused_and_returns_through_existing_back_navigation(native_page):
  path, source = system_source()
  layout = next(node for node in source.body if isinstance(node, ast.ClassDef) and node.name == 'StarPilotSystemLayout')
  open_page = next(node for node in layout.body if isinstance(node, ast.FunctionDef) and node.name == 'open_screen_gaze_settings')
  panel_path = path.with_name('panel.py')
  panel = next(node for node in ast.parse(panel_path.read_text()).body if isinstance(node, ast.ClassDef) and node.name == 'StarPilotPanel')
  navigation = [node for node in panel.body if isinstance(node, ast.FunctionDef) and node.name in ('_navigate_to', '_go_back')]
  namespace = {}
  exec(compile(ast.Module(body=[open_page, *navigation], type_ignores=[]), str(path), 'exec'), namespace)
  entered, backed = [], []
  controller = SimpleNamespace(_params=native_page.cache, _sub_panels={}, _current_sub_panel='',
                               _navigate_callback=entered.append, _back_callback=lambda: backed.append(True))
  controller._navigate_to = lambda target: namespace['_navigate_to'](controller, target)
  assert not controller._sub_panels
  namespace['open_screen_gaze_settings'](controller)
  page = controller._sub_panels['screen_gaze']
  assert controller._current_sub_panel == 'screen_gaze'
  assert entered == ['screen_gaze']
  page._toggle_advanced()
  assert page._advanced_expanded
  namespace['_go_back'](controller)
  assert controller._current_sub_panel == ''
  assert backed == [True]
  namespace['open_screen_gaze_settings'](controller)
  assert controller._sub_panels['screen_gaze'] is page
  assert not page._advanced_expanded
  assert entered == ['screen_gaze', 'screen_gaze']


@pytest.mark.parametrize('width,height', [(1400, 850), (1400, 400), (900, 850)])
def test_display_navigation_row_is_measured_inside_group_and_short_layouts_can_scroll(system_view, width, height):
  view = system_view.view
  view._scroll_rect = system_view.aethergrid.rl.Rectangle(0, 0, width, height)
  view._content_height = view._measure_content_height(width)
  view._draw_basics_tab(0, 0, width)
  assert len(system_view.rows) == 1
  navigation_rect, _ = system_view.rows[0]
  display_group = system_view.groups[0]
  assert navigation_rect.height >= 80
  assert navigation_rect.y + navigation_rect.height <= display_group.y + display_group.height + 1e-6
  if width >= 1180:
    assert max(bottom for _key, bottom in system_view.sliders) <= display_group.y + display_group.height + 1e-6
  if view._content_height > height:
    assert not view.vertical_scrolling_disabled


@pytest.mark.parametrize('modal', ['numeric', 'side', 'style', 'alert'])
@pytest.mark.parametrize('resume_target', ['parent', 'numeric', 'side', 'style'])
def test_embedded_page_widget_ignores_modal_touches_and_resumes_after_cancel(native_page, monkeypatch, modal, resume_target):
  import pyray as rl

  module = importlib.import_module('openpilot.selfdrive.ui.layouts.settings.starpilot.screen_gaze_settings')
  page, storage = native_page.page, native_page.storage
  # The page is embedded in the settings root, not pushed onto the nav stack.
  # Root.disable() when opening a dialog cannot guard this child's events.
  stack = [page._controller]
  monkeypatch.setattr(module.gui_app, '_nav_stack', stack)
  monkeypatch.setattr(module.gui_app, '_show_touches', False)
  monkeypatch.setattr(module.device, '_awake', True)

  def push(dialog):
    native_page.pushed.append(dialog)
    stack.append(dialog)

  monkeypatch.setattr(module.gui_app, 'push_widget', push)
  monkeypatch.setattr(module, 'alert_dialog', lambda message: SimpleNamespace(message=message))
  # Avoid GPU drawing, while retaining Widget.render's real input dispatch,
  # Aether target resolution and the page's actual enabled predicate.
  monkeypatch.setattr(page, '_render', lambda _rect: None)
  bounds = rl.Rectangle(0, 0, 600, 500)
  page._scroll_rect = bounds
  parent_id = f'parent_toggle:{page._parent_toggle.label}'
  page._interactive_rects.update({
    parent_id: rl.Rectangle(20, 20, 200, 60),
    'value:ScreenGazeDimPercent': rl.Rectangle(20, 120, 200, 60),
    'value:ScreenGazeScreenSide': rl.Rectangle(20, 240, 200, 60),
    'value:ScreenGazeDimmingMode': rl.Rectangle(20, 360, 200, 60),
  })
  positions = {'parent': rl.Vector2(100, 50), 'numeric': rl.Vector2(100, 150),
               'side': rl.Vector2(100, 270), 'style': rl.Vector2(100, 390)}

  def tap(target):
    for pressed, released in ((True, False), (False, True)):
      event = SimpleNamespace(slot=0, pos=positions[target], left_pressed=pressed, left_released=released)
      monkeypatch.setattr(module.gui_app, '_mouse_events', [event])
      page.render(bounds)

  assert page not in stack
  assert page.enabled
  if modal == 'numeric':
    page._show_numeric('ScreenGazeDimPercent')
  elif modal == 'side':
    page._show_screen_side()
  elif modal == 'style':
    page._show_dimming_style()
  else:
    assert not page._save('ScreenGazeDimPercent', 2)
  dialog = stack[-1]
  assert not page.enabled
  before = dict(storage.values)
  opened = len(native_page.pushed)
  for target in positions:
    tap(target)
  assert storage.values == before
  assert len(native_page.pushed) == opened
  assert stack[-1] is dialog

  # Actual dialogs remove themselves before invoking their cancel callback.
  stack.pop()
  if modal == 'numeric':
    dialog.callback(DialogResult.CANCEL, 60)
  elif modal in ('side', 'style'):
    dialog.callback(DialogResult.CANCEL)
  assert page.enabled
  tap(resume_target)
  if resume_target == 'parent':
    assert storage.values['ScreenGazeDimming'] is False
    assert len(native_page.pushed) == opened
  else:
    assert storage.values == before
    assert len(native_page.pushed) == opened + 1
    assert isinstance(stack[-1], SliderDialog if resume_target == 'numeric' else SideDialog)
