from threading import Condition, Event, Thread, Timer
import time

import pytest

from openpilot.starpilot.common import screen_brightness
from openpilot.starpilot.common.screen_brightness import ScreenBrightnessWriter


@pytest.fixture
def threads(monkeypatch):
  created = []

  def create(*args, **kwargs):
    thread = Thread(*args, **kwargs)
    created.append(thread)
    return thread

  monkeypatch.setattr(screen_brightness, "Thread", create)
  return created


def test_latest_target_coalesces_while_callback_is_blocked(threads):
  entered, release, complete = Event(), Event(), Event()
  writes = []

  def write(value):
    writes.append(value)
    if value == 10:
      entered.set()
      assert release.wait(2)
    else:
      complete.set()

  writer = ScreenBrightnessWriter(write, interval=0.001)
  try:
    writer.submit(10)
    assert entered.wait(2)
    for value in (20, 30, 40):
      writer.submit(value)
    release.set()
    assert complete.wait(2)
  finally:
    release.set()
    writer.close()
  assert writes == [10, 40]
  assert len(threads) == 1


def test_urgent_upgrade_bypasses_wait_for_same_pending_target(threads):
  first, complete = Event(), Event()
  writes = []

  def write(value):
    writes.append(value)
    (first if value == 10 else complete).set()

  writer = ScreenBrightnessWriter(write, interval=5)
  try:
    writer.submit(10)
    assert first.wait(2)
    writer.submit(20)
    writer.submit(20, urgent=True)
    assert complete.wait(2)
  finally:
    writer.close()
  assert writes == [10, 20]
  assert len(threads) == 1


def test_new_urgent_request_survives_older_inflight_completion():
  entered, release, complete = Event(), Event(), Event()
  writes = []

  def write(value):
    writes.append(value)
    if value == 10:
      entered.set()
      assert release.wait(2)
    else:
      complete.set()

  writer = ScreenBrightnessWriter(write, interval=5)
  try:
    writer.submit(10)
    assert entered.wait(2)
    writer.submit(80, urgent=True)
    release.set()
    assert complete.wait(2)
  finally:
    release.set()
    writer.close()
  assert writes == [10, 80]


def test_reversion_to_previous_applied_value_is_not_lost_inflight():
  first, entered, release, reverted = Event(), Event(), Event(), Event()
  writes = []

  def write(value):
    writes.append(value)
    if value == 20:
      entered.set()
      assert release.wait(2)
    elif len(writes) == 1:
      first.set()
    else:
      reverted.set()

  writer = ScreenBrightnessWriter(write, interval=0.001)
  try:
    writer.submit(10)
    assert first.wait(2)
    writer.submit(20, urgent=True)
    assert entered.wait(2)
    writer.submit(10)
    release.set()
    assert reverted.wait(2)
  finally:
    release.set()
    writer.close()
  assert writes == [10, 20, 10]


def test_regular_write_deadline_coalesces_replacements():
  first, complete = Event(), Event()
  writes = []
  times = []

  def write(value):
    writes.append(value)
    times.append(time.monotonic())
    (first if value == 10 else complete).set()

  writer = ScreenBrightnessWriter(write, interval=0.1)
  try:
    writer.submit(10)
    assert first.wait(2)
    for value in (20, 30, 40):
      writer.submit(value)
    assert complete.wait(2)
  finally:
    writer.close()
  assert writes == [10, 40]
  # This compares the actual two callback times; scheduler delays can only
  # increase the gap, unlike a timing-sensitive sleep-then-assert test.
  assert times[1] - times[0] >= 0.09


def test_regular_update_wakes_worker_that_just_became_idle(monkeypatch):
  idle, first, second = Event(), Event(), Event()

  class ObservedCondition(Condition):
    def wait(self, timeout=None):
      if timeout is not None and timeout > 1:
        idle.set()
      return super().wait(timeout)

  monkeypatch.setattr(screen_brightness, "Condition", ObservedCondition)
  writes = []

  def write(value):
    writes.append(value)
    (first if value == 10 else second).set()

  writer = ScreenBrightnessWriter(write, interval=0.05, idle_timeout=5)
  try:
    writer.submit(10)
    assert first.wait(2)
    # The observer signals while holding the condition lock. submit() cannot
    # acquire it until wait() has atomically released it, avoiding a sleep race.
    assert idle.wait(2)
    writer.submit(20)
    assert second.wait(2)
  finally:
    writer.close()
  assert writes == [10, 20]


def test_slow_callback_cannot_cause_regular_write_burst_on_completion():
  entered, release, complete = Event(), Event(), Event()
  first_completed, second_started = [], []

  def write(value):
    if value == 10:
      entered.set()
      assert release.wait(2)
      first_completed.append(time.monotonic())
    else:
      second_started.append(time.monotonic())
      complete.set()

  writer = ScreenBrightnessWriter(write, interval=0.05)
  timer = Timer(0.08, release.set)
  try:
    writer.submit(10)
    assert entered.wait(2)
    writer.submit(20)
    timer.start()
    assert complete.wait(2)
  finally:
    release.set()
    timer.cancel()
    writer.close()
  # Measure the actual callback boundaries; scheduling delays only widen this
  # interval. A deadline anchored before a slow callback would allow a burst.
  assert second_started[0] - first_completed[0] >= 0.04


def test_repeated_identical_target_does_not_write_again():
  complete = Event()
  writes = []

  def write(value):
    writes.append(value)
    complete.set()

  writer = ScreenBrightnessWriter(write)
  try:
    writer.submit(35)
    assert complete.wait(2)
    for _ in range(100):
      writer.submit(35, urgent=True)
  finally:
    writer.close()
  assert writes == [35]


def test_idle_worker_retires_and_next_change_restarts_it(threads):
  first, second = Event(), Event()
  writes = []

  def write(value):
    writes.append(value)
    (first if value == 10 else second).set()

  writer = ScreenBrightnessWriter(write, interval=0.001, idle_timeout=0.01)
  try:
    assert threads == []
    writer.submit(10)
    assert first.wait(2)
    threads[0].join(2)
    assert not threads[0].is_alive()
    writer.submit(20)
    assert second.wait(2)
  finally:
    writer.close()
  assert len(threads) == 2
  assert writes == [10, 20]


def test_failed_callback_retries_without_repeated_urgent_busy_loop(caplog):
  failed, complete = Event(), Event()
  attempts = []

  def write(_value):
    attempts.append(time.monotonic())
    if len(attempts) == 1:
      failed.set()
      raise OSError("transient write failure")
    complete.set()

  writer = ScreenBrightnessWriter(write, interval=0.001)
  try:
    writer.submit(40, urgent=True)
    assert failed.wait(2)
    for _ in range(100):
      writer.submit(40, urgent=True)
    assert complete.wait(2)
  finally:
    writer.close()
  assert len(attempts) == 2
  assert attempts[1] - attempts[0] >= 0.09
  assert sum("callback failed" in record.message for record in caplog.records) == 1


def test_failed_callback_retries_the_latest_target():
  failed, complete = Event(), Event()
  writes = []

  def write(value):
    writes.append(value)
    if value == 10:
      failed.set()
      raise OSError("transient write failure")
    complete.set()

  writer = ScreenBrightnessWriter(write, interval=0.001)
  try:
    writer.submit(10)
    assert failed.wait(2)
    writer.submit(20)
    assert complete.wait(2)
  finally:
    writer.close()
  assert writes == [10, 20]


@pytest.mark.parametrize("first_urgent, next_target", [(False, 10), (False, 20), (True, 20)])
def test_new_urgent_request_does_not_bypass_failure_backoff(first_urgent, next_target):
  failed, complete = Event(), Event()
  writes, times = [], []

  def write(value):
    writes.append(value)
    times.append(time.monotonic())
    if len(writes) == 1:
      failed.set()
      raise OSError("transient write failure")
    complete.set()

  writer = ScreenBrightnessWriter(write, interval=0.001)
  try:
    writer.submit(10, urgent=first_urgent)
    assert failed.wait(2)
    writer.submit(next_target, urgent=True)
    assert complete.wait(2)
  finally:
    writer.close()
  assert writes == [10, next_target]
  assert times[1] - times[0] >= 0.09


def test_partial_write_failure_reapplies_reverted_previous_level():
  first, entered, release, reverted = Event(), Event(), Event(), Event()
  writes = []

  def write(value):
    writes.append(value)
    if value == 20:
      entered.set()
      assert release.wait(2)
      raise OSError("callback failed after changing the hardware")
    elif len(writes) == 1:
      first.set()
    else:
      reverted.set()

  writer = ScreenBrightnessWriter(write, interval=0.001)
  try:
    writer.submit(10)
    assert first.wait(2)
    writer.submit(20, urgent=True)
    assert entered.wait(2)
    writer.submit(10)
    release.set()
    assert reverted.wait(2)
  finally:
    release.set()
    writer.close()
  assert writes == [10, 20, 10]


def test_close_discards_pending_work_and_prevents_more_submissions():
  entered, release = Event(), Event()
  writes = []

  def write(value):
    writes.append(value)
    entered.set()
    assert release.wait(2)

  writer = ScreenBrightnessWriter(write, interval=0.001)
  try:
    writer.submit(10)
    assert entered.wait(2)
    writer.submit(20)
    writer.close(wait=False)
    with pytest.raises(RuntimeError, match="closed"):
      writer.submit(30)
  finally:
    release.set()
    writer.close()
  assert writes == [10]
