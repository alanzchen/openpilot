"""On-device configuration for the shared gaze-dimming preferences."""

import math

from openpilot.common.params import UnknownKeyName
from openpilot.selfdrive.ui.layouts.settings.starpilot.aethergrid import (
  AetherSettingsView, AetherSliderDialog, ParentToggle, SettingRow, SettingSection,
)
from openpilot.selfdrive.ui.ui_state import device
from openpilot.starpilot.common.screen_settings import SCREEN_GAZE_RANGES, write_screen_setting
from openpilot.system.ui.lib.application import gui_app
from openpilot.system.ui.lib.multilang import tr, tr_noop
from openpilot.system.ui.widgets import DialogResult
from openpilot.system.ui.widgets.confirm_dialog import alert_dialog
from openpilot.system.ui.widgets.option_dialog import MultiOptionDialog


class ScreenGazeSettingsView(AetherSettingsView):
  def __init__(self, controller):
    self._params = controller._params
    self._advanced_expanded = False
    self._dialog = None
    basic_rows = [
      SettingRow(
        "ScreenGazeDimPercent", "value", tr_noop("Dimmed Brightness"),
        tr_noop("Percentage of normal onroad brightness while looking away."),
        enabled=self._active,
        get_value=lambda: f"{self._value('ScreenGazeDimPercent')}%",
        on_click=lambda: self._show_numeric("ScreenGazeDimPercent"),
      ),
      SettingRow(
        "ScreenGazeDimDelay", "value", tr_noop("Looking Away Delay"),
        tr_noop("Wait before dimming. Looking toward the screen restores brightness immediately."),
        enabled=self._active,
        get_value=lambda: tr("{} seconds").format(self._value("ScreenGazeDimDelay")),
        on_click=lambda: self._show_numeric("ScreenGazeDimDelay"),
      ),
      SettingRow(
        "ScreenGazeScreenSide", "value", tr_noop("Screen Side"),
        tr_noop("Where the screen is mounted, from the driver's seat."),
        enabled=self._active,
        get_value=lambda: tr("Left") if self._value("ScreenGazeScreenSide") else tr("Right"),
        on_click=self._show_screen_side,
      ),
      SettingRow(
        "advanced", "value", tr_noop("Advanced"),
        tr_noop("Adjust how far to turn your head toward the screen."),
        enabled=self._active,
        get_value=lambda: tr("Hide") if self._advanced_expanded else tr("Show"),
        on_click=self._toggle_advanced,
      ),
      SettingRow(
        "ScreenGazeYawThreshold", "value", tr_noop("Screen Look Angle"),
        tr_noop("Head turn from forward toward the screen. Lower for a smaller glance."),
        visible=lambda: self._advanced_expanded,
        enabled=lambda: self._active() and self._advanced_expanded,
        get_value=lambda: f"{self._value('ScreenGazeYawThreshold')}°",
        on_click=lambda: self._show_numeric("ScreenGazeYawThreshold"),
      ),
    ]
    sections = [
      SettingSection("", [
        SettingRow(
          "ScreenManagement", "toggle", tr_noop("Screen Settings"),
          tr_noop("Turn on Screen Settings to use custom brightness and dimming."),
          enabled=lambda: not self._screen_management_enabled(),
          get_state=self._screen_management_enabled,
          set_state=lambda enabled: self._save("ScreenManagement", enabled),
        ),
      ], visible=lambda: not self._screen_management_enabled()),
      SettingSection("", basic_rows, visible=self._active),
    ]
    super().__init__(
      controller, sections,
      header_title=tr_noop("Dim When Looking Away"),
      header_subtitle=tr_noop(
        "Uses head direction while driving. Touch and alerts restore brightness. Standby and manual screen-off can still turn the display off."
      ),
      parent_toggle=ParentToggle(
        label=tr_noop("Dim When Looking Away"),
        get_state=self._gaze_enabled,
        set_state=self._set_enabled,
      ),
    )
    # The settings page is embedded below the navigation-stack root. Disabling
    # that root for a modal does not disable this child's touch handlers.
    self.set_enabled(lambda: not self._dialog_open())

  def _dialog_open(self) -> bool:
    return self._dialog is not None and gui_app.widget_in_stack(self._dialog)

  def _push_dialog(self, dialog):
    if self._dialog_open():
      return
    self._dialog = dialog
    gui_app.push_widget(dialog)

  def show_event(self):
    # Galaxy can change these keys while this page is closed.
    self._params.invalidate()
    self._advanced_expanded = False
    super().show_event()

  def _screen_management_enabled(self) -> bool:
    return self._params.get_bool("ScreenManagement", default=True)

  def _gaze_enabled(self) -> bool:
    return self._params.get_bool("ScreenGazeDimming")

  def _active(self) -> bool:
    return self._screen_management_enabled() and self._gaze_enabled()

  def _value(self, key: str) -> int:
    default, minimum, maximum = SCREEN_GAZE_RANGES[key]
    try:
      value = float(self._params.get(key))
      if math.isfinite(value):
        return min(maximum, max(minimum, round(value)))
    except (TypeError, ValueError, OverflowError, KeyError, UnknownKeyName):
      pass
    return default

  def _save(self, key: str, value) -> bool:
    try:
      write_screen_setting(self._params, key, value)
    except UnknownKeyName:
      self._push_dialog(alert_dialog(tr(
        "This installation needs rebuilt device binaries for screen dimming. Install a build that includes these settings."
      )))
      return False
    except (OSError, ValueError, KeyError):
      self._push_dialog(alert_dialog(tr("Screen setting could not be saved. Please try again.")))
      return False
    self._params.invalidate()
    device.reset_interactive_timeout()
    return True

  def _set_enabled(self, enabled: bool):
    if enabled and not self._screen_management_enabled():
      self._push_dialog(alert_dialog(tr("Enable Screen Settings below before turning on gaze dimming.")))
      return
    self._save("ScreenGazeDimming", enabled)

  def _toggle_advanced(self):
    self._advanced_expanded = not self._advanced_expanded

  def _show_numeric(self, key: str):
    specs = {
      "ScreenGazeDimPercent": (tr("Dimmed Brightness"), "%"),
      "ScreenGazeDimDelay": (tr("Looking Away Delay"), tr(" seconds")),
      "ScreenGazeYawThreshold": (tr("Screen Look Angle"), "°"),
    }
    title, unit = specs[key]
    _, minimum, maximum = SCREEN_GAZE_RANGES[key]

    def on_close(result, value):
      if result == DialogResult.CONFIRM:
        self._save(key, int(value))

    self._push_dialog(AetherSliderDialog(
      title=title, min_val=minimum, max_val=maximum, step=1,
      current_val=self._value(key), on_close=on_close, unit=unit,
    ))

  def _show_screen_side(self):
    options = [tr("Right"), tr("Left")]

    def on_select(result):
      if result == DialogResult.CONFIRM and dialog.selection in options:
        self._save("ScreenGazeScreenSide", options.index(dialog.selection))

    dialog = MultiOptionDialog(
      tr("Screen Side"), options, options[self._value("ScreenGazeScreenSide")], callback=on_select,
    )
    self._push_dialog(dialog)
