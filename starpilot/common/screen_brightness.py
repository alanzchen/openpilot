"""Bounded, latest-value delivery of display brightness to a slow backend."""
from collections.abc import Callable
import logging
import math
from threading import Condition, Thread, current_thread
import time


class ScreenBrightnessWriter:
  """Coalesce updates into one lazy worker, with no queue of obsolete levels.

  Regular writes start at most once per interval. An urgent request can bypass
  that deadline once for its current target; repeated identical urgent requests
  cannot turn a failing callback into a busy loop. Callback exceptions retry the
  latest target after a backoff. A normal return is only callback completion,
  not independent verification that a device accepted the physical write.
  """

  def __init__(self, write_callback: Callable[[int], None], *, interval: float = 1 / 30, idle_timeout: float = 0.25):
    if not math.isfinite(interval) or interval <= 0 or not math.isfinite(idle_timeout) or idle_timeout <= 0:
      raise ValueError("Brightness writer intervals must be positive and finite")
    self._write = write_callback
    self._interval = interval
    self._idle_timeout = idle_timeout
    self._condition = Condition()
    self._target: int | None = None
    self._applied: int | None = None
    self._generation = 0
    self._urgent_generation: int | None = None
    self._attempted_urgent_generation: int | None = None
    self._next_write_at = 0.0
    self._retry_at = 0.0
    self._thread: Thread | None = None
    self._closed = False

  def submit(self, brightness: int, *, urgent: bool = False) -> None:
    if type(brightness) is not int or not 0 <= brightness <= 100:
      raise ValueError("Brightness must be an integer from 0 to 100")
    with self._condition:
      if self._closed:
        raise RuntimeError("Brightness writer is closed")
      was_idle = self._target == self._applied
      changed = brightness != self._target
      if changed:
        self._generation += 1
        self._target = brightness
        self._urgent_generation = self._generation if urgent else None
      elif urgent:
        self._urgent_generation = self._generation
      if self._target == self._applied and not changed:
        return

      if self._thread is None:
        self._thread = Thread(target=self._run, name="screen-brightness", daemon=True)
        try:
          self._thread.start()
        except BaseException:
          self._thread = None
          raise
      elif (self._urgent_generation == self._generation and self._attempted_urgent_generation != self._generation) or was_idle:
        # Wake an idle worker for its next deadline, even if the previous
        # write was recent. Pending regular replacements need no extra wake.
        self._condition.notify()

  def close(self, *, wait: bool = True) -> None:
    """Discard pending work. Optionally wait for an already-running write."""
    with self._condition:
      self._closed = True
      thread = self._thread
      self._condition.notify()
    if wait and thread is not None and thread is not current_thread():
      thread.join()

  def _run(self) -> None:
    failure_logged = False
    try:
      while True:
        with self._condition:
          if self._closed:
            return
          if self._target == self._applied:
            idle_deadline = time.monotonic() + self._idle_timeout
            while not self._closed and self._target == self._applied:
              remaining = idle_deadline - time.monotonic()
              if remaining <= 0:
                # Retirement and submission share this lock: no request can
                # arrive between deciding to exit and publishing that decision.
                self._thread = None
                return
              self._condition.wait(remaining)
            continue

          generation = self._generation
          urgent = self._urgent_generation == generation and self._attempted_urgent_generation != generation
          now = time.monotonic()
          deadline = max(self._retry_at, 0.0 if urgent else self._next_write_at)
          if now < deadline:
            self._condition.wait(deadline - now)
            continue
          target = self._target
          if urgent:
            self._attempted_urgent_generation = generation

        # The UI must never wait on a sysfs operation while holding this lock.
        try:
          self._write(target)
        except Exception:
          with self._condition:
            # A callback can raise after partially completing a write. The
            # previous level is no longer known, so even a reversion needs I/O.
            self._applied = None
            completed_at = time.monotonic()
            self._next_write_at = completed_at + self._interval
            self._retry_at = completed_at + max(0.1, self._interval)
          if not failure_logged:
            logging.getLogger(__name__).exception("Screen brightness callback failed; retrying latest target")
            failure_logged = True
        else:
          failure_logged = False
          with self._condition:
            self._applied = target
            # Anchor to completion: a preemption immediately before the call
            # must not permit a burst of two regular hardware writes on resume.
            self._next_write_at = time.monotonic() + self._interval
            self._retry_at = 0.0
    finally:
      with self._condition:
        # An idle worker can have retired and been replaced before its finally
        # clause runs. Never erase the replacement worker's reference.
        if self._thread is current_thread():
          self._thread = None
