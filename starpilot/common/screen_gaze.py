"""Display-only head-pose dimming. Never changes driver-monitoring policy."""
import math

from openpilot.starpilot.common.screen_settings import GazeDimmingPreferences


def screen_yaw(dm, screen_side: int) -> float | None:
  """Degrees toward the configured screen, or None when vision is unreliable.

  DM corrects camera position/calibration and mirrors yaw for RHD. Undo that
  mirror after subtracting its neutral pose so Right/Left mean physical sides.
  This estimates a head turn, not eye gaze or attention to a particular object.
  """
  if dm is None or str(dm.activePolicy) not in ('vision', '1'):
    return None
  vision = dm.visionPolicyState
  if not vision.faceDetected:
    return None
  pose = vision.pose
  yaw, uncertainty = pose.yaw, pose.uncertainty
  neutral = pose.yawCalib.offset if pose.calibrated else 0.075
  if not all(math.isfinite(v) for v in (yaw, neutral, uncertainty)) or not 0 <= uncertainty < 0.3:
    return None
  # Match the bounds used by the driver-monitoring policy's neutral pose.
  neutral = min(0.289, max(-0.0246, neutral))
  yaw = math.degrees(yaw - neutral) * (-1 if dm.isRHD else 1)
  return yaw * (-1 if screen_side == 1 else 1)


class ScreenGazeDimmer:
  FOLLOW_SLEW_PER_SECOND = 4.0
  FOLLOW_TIME_CONSTANT = 0.05
  FOLLOW_MAX_FRAME_TIME = 0.05
  FOLLOW_RESET_GAP = 0.5

  def __init__(self):
    self.reset()

  def reset(self):
    self._away_since: float | None = None
    self._looking = False
    self._mode: int | None = None
    self._fraction = 1.0
    self._last_time: float | None = None

  def brightness(self, normal: int, now: float, preferences: GazeDimmingPreferences,
                 yaw: float | None, *, suppressed: bool = False) -> int:
    if not preferences.enabled or suppressed or yaw is None or not math.isfinite(yaw):
      self.reset()
      return normal

    mode = 1 if preferences.mode == 1 else 0
    if mode != self._mode:
      self.reset()
      self._mode = mode
    if mode == 1:
      return self._follow_brightness(normal, now, preferences, yaw)

    # Hysteresis and a continuous look-away delay avoid brightness flicker.
    threshold = preferences.yaw_threshold - (3 if self._looking else 0)
    self._looking = yaw >= threshold
    if self._looking:
      self._away_since = None
    elif self._away_since is None:
      self._away_since = now

    if self._away_since is None or now - self._away_since < preferences.delay:
      return normal
    # Never brighten an intentionally dark display, or dim to a blank screen.
    return min(normal, max(5, round(normal * preferences.percent / 100)))

  def _follow_brightness(self, normal: int, now: float, preferences: GazeDimmingPreferences, yaw: float) -> int:
    if not math.isfinite(now):
      self.reset()
      return normal
    previous = self._last_time
    self._last_time = now
    # Start bright after activation, a clock reset, or a stalled UI. Bound the
    # next step as well so a dropped frame cannot create a large visible jump.
    if previous is None or not 0 <= now - previous <= self.FOLLOW_RESET_GAP:
      self._fraction = 1.0
      return normal
    elapsed = min(now - previous, self.FOLLOW_MAX_FRAME_TIME)
    position = min(1.0, max(0.0, yaw / max(1, preferences.yaw_threshold)))
    smooth = position * position * (3.0 - 2.0 * position)
    floor = preferences.percent / 100
    target = floor + (1.0 - floor) * smooth
    step = self.FOLLOW_SLEW_PER_SECOND * elapsed
    # Raw DM pose can jitter between packets. A fast first-order response
    # attenuates those small changes; the slew bound also softens large turns.
    change = (target - self._fraction) * elapsed / (self.FOLLOW_TIME_CONSTANT + elapsed)
    self._fraction += min(step, max(-step, change))
    return min(normal, max(5, round(normal * self._fraction)))
