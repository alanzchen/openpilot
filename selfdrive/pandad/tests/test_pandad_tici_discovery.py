"""comma 3 (tici) internal panda discovery and recovery ordering (mazda-ti layer).

Import-time isolation: logging, Params, hardware and the native pandad API are replaced in sys.modules before
pandad is imported, so no device hardware, production Params or log paths are touched. Run with --noconftest
(the repository conftest manages real device processes)."""
import importlib
import subprocess
import sys
import types

import pytest

PANDAD = "openpilot.selfdrive.pandad.pandad"
STUBBED = ("openpilot.common.swaglog", "openpilot.common.params", "openpilot.system.hardware",
           "openpilot.selfdrive.pandad.pandad_api_impl")


class _Stop(BaseException):
  """Escapes pandad.main()'s `except Exception` handlers once the scenario is decided."""


class FakeHardware:
  def __init__(self, device_type="tici"):
    self.device_type = device_type
    self.calls = []
    self.on_recover = None

  def get_device_type(self):
    return self.device_type

  def reset_internal_panda(self):
    self.calls.append("reset")

  def recover_internal_panda(self):
    self.calls.append("recover")
    if self.on_recover:
      self.on_recover()

  def has_internal_panda(self):
    return True


class FakeParams:
  def __init__(self, *args, **kwargs):
    pass

  def remove(self, key):
    pass

  def get_bool(self, key, *args, **kwargs):
    return False

  def get(self, key, *args, **kwargs):
    return None

  def put(self, *args, **kwargs):
    pass

  def put_bool(self, *args, **kwargs):
    pass


class Device:
  """The internal panda as seen through sysfs, udev and libusb."""
  def __init__(self):
    self.present = True
    self.devnum = "5"
    self.serial = "internal"
    self.udev_ready = False
    self.udev_rc = 0
    self.udev_exc = None
    self.listable = True       # libusb can open it once udev applied permissions
    self.reenumerate_on_reset = True
    self.udev_calls = 0
    self.external = []         # serials of external USB pandas
    self.app_mode = True       # False while the internal panda sits in DFU


@pytest.fixture
def env(monkeypatch):
  logs = []
  log = types.SimpleNamespace(**{lvl: (lambda *a, _l=lvl, **k: logs.append((_l, " ".join(map(str, a)))))
                                 for lvl in ("debug", "info", "warning", "error", "event", "exception")})
  hw = FakeHardware()
  stubs = {
    "openpilot.common.swaglog": types.SimpleNamespace(cloudlog=log),
    "openpilot.common.params": types.SimpleNamespace(Params=FakeParams, UnknownKeyName=KeyError),
    "openpilot.system.hardware": types.SimpleNamespace(HARDWARE=hw, PC=False, TICI=True),
    "openpilot.selfdrive.pandad.pandad_api_impl": types.SimpleNamespace(can_list_to_can_capnp=lambda *a: None, can_capnp_to_list=lambda *a: None),
  }
  for name, stub in stubs.items():
    monkeypatch.setitem(sys.modules, name, stub)
  for name in list(sys.modules):
    if name.startswith("openpilot.selfdrive.pandad"):
      monkeypatch.delitem(sys.modules, name)
  for name, stub in stubs.items():
    monkeypatch.setitem(sys.modules, name, stub)
  pandad = importlib.import_module(PANDAD)
  assert pandad.cloudlog is log and pandad.HARDWARE is hw

  dev = Device()
  real_exists = pandad.os.path.exists
  monkeypatch.setattr(pandad.os.path, "exists",
                      lambda p: dev.present if p == pandad.TICI_INTERNAL_PANDA_USB_DEVICE else real_exists(p))
  monkeypatch.setattr(pandad, "_tici_usb_attr",
                      lambda name: (dev.devnum if name == "devnum" else dev.serial) if dev.present else None)
  monkeypatch.setattr(pandad.time, "sleep", lambda s: None)

  def fake_run(cmd, **kwargs):
    assert cmd[:2] == ["udevadm", "wait"] and cmd[-1] == pandad.TICI_INTERNAL_PANDA_USB_DEVICE
    dev.udev_calls += 1
    if dev.udev_exc:
      raise dev.udev_exc
    if dev.udev_rc == 0 and dev.present:
      dev.udev_ready = True
    return subprocess.CompletedProcess(cmd, dev.udev_rc)
  monkeypatch.setattr(pandad.subprocess, "run", fake_run)

  def reset():
    hw.calls.append("reset")
    dev.udev_ready = False
    if dev.reenumerate_on_reset and dev.present:
      dev.devnum = str(int(dev.devnum) + 1)
  monkeypatch.setattr(hw, "reset_internal_panda", reset)

  def panda_list(usb_only=False):
    assert usb_only is (hw.device_type == "tici")
    internal = [dev.serial] if (dev.present and dev.udev_ready and dev.listable and dev.app_mode) else []
    return internal + list(dev.external)
  monkeypatch.setattr(pandad.Panda, "list", staticmethod(panda_list))
  monkeypatch.setattr(pandad.PandaDFU, "usb_list", staticmethod(lambda: []))
  monkeypatch.setattr(pandad.PandaDFU, "list", staticmethod(lambda: pytest.fail("tici must not probe SPI DFU")))
  monkeypatch.setattr(pandad, "Params", FakeParams)
  monkeypatch.setattr(pandad, "prepare_rivian_bridge", lambda serials: [])
  return types.SimpleNamespace(pandad=pandad, hw=hw, dev=dev, logs=logs)


def run_main(env, monkeypatch, max_iterations=12):
  """Run pandad.main() until the panda gets flashed (success) or max_iterations scans happened."""
  pandad = env.pandad
  scans = {"n": 0}
  real_list = pandad.list_pandas

  def counted_list():
    scans["n"] += 1
    if scans["n"] > max_iterations:
      raise _Stop("gave up")
    return real_list()
  monkeypatch.setattr(pandad, "list_pandas", counted_list)

  def flash(*args, **kwargs):
    raise _Stop("flashed")
  monkeypatch.setattr(pandad, "flash_panda", flash)
  with pytest.raises(_Stop) as e:
    pandad.main()
  return str(e.value)


def warnings_of(env):
  return [msg for lvl, msg in env.logs if lvl == "warning"]


def test_initial_scan_waits_for_udev_and_never_resets(env, monkeypatch):
  assert run_main(env, monkeypatch) == "flashed"
  assert env.hw.calls == []


def test_panda_appearing_after_first_scan_is_not_reset(env, monkeypatch):
  env.dev.present = False
  pandad = env.pandad
  real = pandad.list_pandas
  def appear_after_first(*a, **k):
    out = real()
    env.dev.present = True  # shows up right after the first (empty) scan
    return out
  monkeypatch.setattr(pandad, "list_pandas", appear_after_first)
  monkeypatch.setattr(pandad, "flash_panda", lambda *a, **k: (_ for _ in ()).throw(_Stop("flashed")))
  with pytest.raises(_Stop, match="flashed"):
    pandad.main()
  assert env.hw.calls == [], "counter=1 with a newly present panda must connect, not reset"


def test_panda_appearing_before_dfu_step_is_not_put_into_dfu(env, monkeypatch):
  env.dev.present = False
  pandad = env.pandad
  scans = {"n": 0}
  real = pandad.list_pandas
  def appear_at_third(*a, **k):
    scans["n"] += 1
    out = real()
    if scans["n"] == 3:
      env.dev.present = True
    return out
  monkeypatch.setattr(pandad, "list_pandas", appear_at_third)
  monkeypatch.setattr(pandad, "flash_panda", lambda *a, **k: (_ for _ in ()).throw(_Stop("flashed")))
  with pytest.raises(_Stop, match="flashed"):
    pandad.main()
  assert env.hw.calls == ["reset", "reset"], "counter=3 with a newly present panda must not enter DFU"


def test_initialized_but_unopenable_progresses_to_dfu(env, monkeypatch):
  env.dev.listable = False
  env.hw.on_recover = lambda: (_ for _ in ()).throw(_Stop("dfu"))
  assert run_main(env, monkeypatch) == "dfu"
  assert env.hw.calls == ["reset", "reset", "recover"]


def test_absent_panda_keeps_original_recovery_order_without_waiting(env, monkeypatch):
  env.dev.present = False
  env.hw.on_recover = lambda: (_ for _ in ()).throw(_Stop("dfu"))
  assert run_main(env, monkeypatch) == "dfu"
  assert env.hw.calls == ["reset", "reset", "recover"]
  assert env.dev.udev_calls == 0


def test_stale_devnum_after_reset_is_bounded(env, monkeypatch):
  env.dev.listable = False
  env.dev.reenumerate_on_reset = False
  env.hw.on_recover = lambda: (_ for _ in ()).throw(_Stop("dfu"))
  assert run_main(env, monkeypatch) == "dfu"
  assert any("did not re-enumerate" in w for w in warnings_of(env))


def test_disappearing_during_wait_is_bounded(env, monkeypatch):
  env.dev.udev_rc = 1
  def vanish(cmd, **kwargs):
    env.dev.present = False
    return subprocess.CompletedProcess(cmd, 1)
  monkeypatch.setattr(env.pandad.subprocess, "run", vanish)
  env.hw.on_recover = lambda: (_ for _ in ()).throw(_Stop("dfu"))
  assert run_main(env, monkeypatch) == "dfu"


@pytest.mark.parametrize("exc, text", [(subprocess.TimeoutExpired(["udevadm"], 35), "did not return in time"),
                                       (FileNotFoundError("udevadm"), "udevadm unavailable")])
def test_udevadm_failures_are_distinct_and_not_fatal(env, exc, text):
  env.dev.udev_exc = exc
  assert env.pandad.wait_for_tici_panda_udev() is False
  assert any(text in w for w in warnings_of(env))


def test_udev_timeout_message(env):
  env.dev.udev_rc = 1
  assert env.pandad.wait_for_tici_panda_udev() is False
  assert any("not initialized by udev" in w for w in warnings_of(env))


def test_post_dfu_recovery_waits_for_udev_again(env, monkeypatch):
  pandad = env.pandad
  dfu = {"listed": False}
  monkeypatch.setattr(pandad.PandaDFU, "usb_list", staticmethod(lambda: [] if dfu["listed"] else (dfu.update(listed=True) or ["dfu"])))
  class FakeDFU:
    def __init__(self, serial):
      pass
    def recover(self):
      env.dev.udev_ready = False   # re-enumerates into application mode
  monkeypatch.setattr(pandad, "PandaDFU", type("PD", (), {"usb_list": staticmethod(pandad.PandaDFU.usb_list),
                                                          "list": staticmethod(lambda: pytest.fail("SPI probe")),
                                                          "__new__": lambda cls, s: FakeDFU(s)}))
  before = env.dev.udev_calls
  assert run_main(env, monkeypatch) == "flashed"
  assert env.dev.udev_calls - before >= 2, "udev wait must run again after the DFU recover"


@pytest.mark.parametrize("device_type", ["tizi", "mici"])
def test_non_tici_unchanged(env, device_type):
  env.hw.device_type = device_type
  assert env.pandad.wait_for_tici_panda_udev() is True
  assert env.pandad.tici_internal_panda_ready() is False
  assert env.pandad.wait_for_tici_panda_reenumeration("5") is True
  assert env.dev.udev_calls == 0


def test_listing_is_usb_only_on_tici(env):
  env.dev.udev_ready = True
  assert env.pandad.list_pandas() == ["internal"]   # the fake asserts usb_only=True on tici
  assert env.pandad.list_dfu_pandas() == []          # PandaDFU.list (SPI) fails the test if called


def test_skip_at_counter3_then_vanish_still_reaches_dfu(env, monkeypatch):
  env.dev.present = False
  pandad = env.pandad
  real = pandad.list_pandas
  state = {"scans": 0, "seen": False}
  def scripted(*a, **k):
    state["scans"] += 1
    if state["scans"] == 3:
      env.dev.present = True        # appears just before the counter-3 pre-check
    out = real()
    if out and not state["seen"]:
      state["seen"] = True
      env.dev.present = False       # ...and vanishes before the application listing
    return out
  monkeypatch.setattr(pandad, "list_pandas", scripted)
  env.hw.on_recover = lambda: (_ for _ in ()).throw(_Stop("dfu"))
  monkeypatch.setattr(pandad, "flash_panda", lambda *a, **k: (_ for _ in ()).throw(_Stop("flashed")))
  with pytest.raises(_Stop, match="dfu"):
    pandad.main()
  assert env.hw.calls == ["reset", "reset", "recover"], "the pending DFU step must survive a skipped attempt"


def _dfu_once(env, monkeypatch, stale):
  pandad = env.pandad
  listed = {"done": False}
  def usb_list():
    if listed["done"]:
      return []
    listed["done"] = True
    return ["dfu-serial"]
  class FakeDFU:
    def __init__(self, serial):
      pass
    def recover(self):
      env.dev.udev_ready = False
      if not stale:
        env.dev.devnum = str(int(env.dev.devnum) + 1)
  fake_cls = type("FakePandaDFU", (), {"usb_list": staticmethod(usb_list),
                                       "list": staticmethod(lambda: pytest.fail("SPI probe")),
                                       "__new__": lambda cls, s: FakeDFU(s)})
  monkeypatch.setattr(pandad, "PandaDFU", fake_cls)


def test_post_dfu_waits_for_new_enumeration_then_udev(env, monkeypatch):
  _dfu_once(env, monkeypatch, stale=False)
  assert run_main(env, monkeypatch) == "flashed"
  assert not any("did not re-enumerate" in w for w in warnings_of(env))


def test_post_dfu_stale_node_is_bounded_and_reported(env, monkeypatch):
  _dfu_once(env, monkeypatch, stale=True)
  assert run_main(env, monkeypatch) == "flashed"
  assert any("did not re-enumerate" in w for w in warnings_of(env))


def test_external_panda_does_not_count_as_internal(env):
  env.dev.external = ["external-1"]
  env.dev.listable = False
  assert env.pandad.tici_internal_panda_ready() is False


def test_internal_dfu_device_is_not_ready(env):
  env.dev.app_mode = False
  assert env.pandad.tici_internal_panda_ready() is False
