"""Display-only head-pose dimming. Never changes driver-monitoring policy."""
import math

from openpilot.starpilot.common.screen_settings import GazeDimmingPreferences


def screen_yaw(dm, screen_side: int) -> float | None:
  """Degrees toward the configured screen, or None when vision is unreliable.

  DM corrects camera position/calibration and mirrors yaw for RHD. Undo that
  mirror after subtracting its neutral pose so Right/Left mean physical sides.
  This estimates a head turn, not eye gaze or attention to a particular object.
  """
  if dm is None or str(dm.activePolicy) not in ('vision', '1') or not dm.visionPolicyState.faceDetected:
    return None
  pose = dm.visionPolicyState.pose
  neutral = pose.yawCalib.offset if pose.calibrated else 0.075
  if not all(math.isfinite(v) for v in (pose.yaw, neutral, pose.uncertainty)) or not 0 <= pose.uncertainty < 0.3:
    return None
  # Match the bounds used by the driver-monitoring policy's neutral pose.
  neutral = min(0.289, max(-0.0246, neutral))
  yaw = math.degrees(pose.yaw - neutral) * (-1 if dm.isRHD else 1)
  return yaw * (-1 if screen_side == 1 else 1)


class ScreenGazeDimmer:
  def __init__(self):
    self.reset()

  def reset(self):
    self._away_since: float | None = None
    self._looking = False

  def brightness(self, normal: int, now: float, preferences: GazeDimmingPreferences,
                 yaw: float | None, *, suppressed: bool = False) -> int:
    if not preferences.enabled or suppressed or yaw is None or not math.isfinite(yaw):
      self.reset()
      return normal

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
