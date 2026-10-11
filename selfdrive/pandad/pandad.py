#!/usr/bin/env python3
# simple pandad wrapper that updates the panda first
import os
import usb1
import time
import signal
import subprocess

from panda import Panda, PandaDFU, PandaProtocolMismatch, FW_PATH
from openpilot.common.basedir import BASEDIR
from openpilot.common.params import Params, UnknownKeyName
from openpilot.system.hardware import HARDWARE
from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.pandad.rivian_long_flasher import prepare_rivian_bridge
from openpilot.selfdrive.pandad.panda_firmware import get_firmware_path, get_tesla_wake_on_can
from openpilot.selfdrive.pandad.panda_firmware import get_selected_firmware_name as get_selected_firmware_name


# comma 3 (tici): the internal panda is a USB device on this port, and udev must finish applying its
# permissions before libusb can open it. Waiting here keeps a slow udev coldplug from being mistaken for a
# missing panda, which used to trigger resets and DFU entry while the car was starting.
TICI_INTERNAL_PANDA_USB_DEVICE = "/sys/bus/usb/devices/1-1.2"
TICI_UDEV_WAIT_TIMEOUT_S = 30
TICI_REENUMERATION_TIMEOUT_S = 10


def is_tici() -> bool:
  return HARDWARE.get_device_type() == "tici"


def _tici_usb_attr(name: str) -> str | None:
  try:
    with open(os.path.join(TICI_INTERNAL_PANDA_USB_DEVICE, name)) as f:
      return f.read().strip()
  except OSError:
    return None


def wait_for_tici_panda_udev(timeout: int = TICI_UDEV_WAIT_TIMEOUT_S) -> bool:
  # Only wait for an enumerated device, so a missing panda still reaches the reset/DFU recovery without delay.
  if not is_tici() or not os.path.exists(TICI_INTERNAL_PANDA_USB_DEVICE):
    return True
  try:
    ret = subprocess.run(["udevadm", "wait", f"--timeout={timeout}", TICI_INTERNAL_PANDA_USB_DEVICE],
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=False, timeout=timeout + 5)
  except subprocess.TimeoutExpired:
    cloudlog.warning("pandad: udevadm wait did not return in time")
    return False
  except OSError as e:
    cloudlog.warning(f"pandad: udevadm unavailable: {e}")
    return False
  if ret.returncode != 0:
    cloudlog.warning("pandad: internal panda USB device not initialized by udev before timeout")
  return ret.returncode == 0


def wait_for_tici_panda_reenumeration(previous_devnum: str | None, timeout: float = TICI_REENUMERATION_TIMEOUT_S) -> bool:
  # After a reset or DFU entry the port must show a new USB device before its udev state means anything.
  if not is_tici():
    return True
  for _ in range(int(timeout / 0.1)):
    devnum = _tici_usb_attr("devnum")
    if devnum is not None and devnum != previous_devnum:
      return True
    time.sleep(0.1)
  cloudlog.warning("pandad: internal panda did not re-enumerate after reset")
  return False


def list_dfu_pandas() -> list[str]:
  # the internal panda is USB-only on comma 3; its SPI device node has no panda
  return PandaDFU.usb_list() if is_tici() else PandaDFU.list()


def list_pandas() -> list[str]:
  return Panda.list(usb_only=is_tici())


def tici_internal_panda_ready() -> bool:
  if not is_tici() or not os.path.exists(TICI_INTERNAL_PANDA_USB_DEVICE) or not wait_for_tici_panda_udev():
    return False
  serial = _tici_usb_attr("serial")
  return serial is not None and serial in list_pandas()


def get_expected_firmware_path(panda: Panda, remote_start: bool, hkg_remote_start: bool, ignore_ignition_line: bool,
                               tesla_wake: bool = False) -> str:
  return get_firmware_path(FW_PATH, panda.get_mcu_type().config.app_fn, remote_start, hkg_remote_start, ignore_ignition_line, tesla_wake)


def get_expected_signature(panda: Panda, remote_start: bool, hkg_remote_start: bool, ignore_ignition_line: bool, tesla_wake: bool = False) -> bytes:
  try:
    fn = get_expected_firmware_path(panda, remote_start, hkg_remote_start, ignore_ignition_line, tesla_wake)
    return Panda.get_signature_from_firmware(fn)
  except Exception:
    cloudlog.exception("Error computing expected signature")
    return b""


def get_remote_start_boots_comma(params: Params) -> bool:
  try:
    return params.get_bool("RemoteStartBootsComma")
  except UnknownKeyName:
    return False


def get_hkg_remote_start_boots_comma(params: Params) -> bool:
  try:
    return params.get_bool("HKGRemoteStartBootsComma")
  except UnknownKeyName:
    return False


def get_ignore_ignition_line(params: Params) -> bool:
  try:
    return params.get_bool("IgnoreIgnitionLine")
  except UnknownKeyName:
    return False


def flash_panda(panda_serial: str, remote_start: bool, hkg_remote_start: bool, ignore_ignition_line: bool, tesla_wake: bool = False) -> Panda:
  try:
    panda = Panda(panda_serial)
  except PandaProtocolMismatch:
    cloudlog.warning("detected protocol mismatch, reflashing panda")
    HARDWARE.recover_internal_panda()
    raise

  fw_path = get_expected_firmware_path(panda, remote_start, hkg_remote_start, ignore_ignition_line, tesla_wake)
  fw_signature = get_expected_signature(panda, remote_start, hkg_remote_start, ignore_ignition_line, tesla_wake)
  internal_panda = panda.is_internal()

  panda_version = "bootstub" if panda.bootstub else panda.get_version()
  panda_signature = b"" if panda.bootstub else panda.get_signature()
  cloudlog.warning(f"Panda {panda_serial} connected, version: {panda_version}, signature {panda_signature.hex()[:16]}, expected {fw_signature.hex()[:16]}")

  if panda.bootstub or panda_signature != fw_signature:
    cloudlog.info("Panda firmware out of date, update required")
    panda.flash(fn=fw_path)
    cloudlog.info("Done flashing")

  if panda.bootstub:
    bootstub_version = panda.get_version()
    cloudlog.info(f"Flashed firmware not booting, flashing development bootloader. {bootstub_version=}, {internal_panda=}")
    if internal_panda:
      HARDWARE.recover_internal_panda()
    panda.recover(reset=(not internal_panda))
    cloudlog.info("Done flashing bootstub")

  if panda.bootstub:
    cloudlog.info("Panda still not booting, exiting")
    raise AssertionError

  panda_signature = panda.get_signature()
  if panda_signature != fw_signature:
    cloudlog.info("Version mismatch after flashing, exiting")
    raise AssertionError

  return panda


def main() -> None:
  # signal pandad to close the relay and exit
  def signal_handler(signum, frame):
    cloudlog.info(f"Caught signal {signum}, exiting")
    nonlocal do_exit
    do_exit = True
    if process is not None:
      process.send_signal(signal.SIGINT)

  process = None
  do_exit = False
  signal.signal(signal.SIGINT, signal_handler)

  count = 0
  first_run = True
  params = Params()
  no_internal_panda_count = 0
  recovery_skipped = False
  recovery_actions = 0

  while not do_exit:
    try:
      count += 1
      cloudlog.event("pandad.flash_and_connect", count=count)
      params.remove("PandaSignatures")

      # Handle missing internal panda. Recovery stages advance by actions actually taken (reset, reset, DFU,
      # then resets), so a skipped attempt never loses the pending DFU step.
      if no_internal_panda_count == 0:
        recovery_skipped = False
        recovery_actions = 0
      elif not recovery_skipped and tici_internal_panda_ready():
        # it appeared since the last scan: connect instead of resetting it (once per missing episode)
        cloudlog.info("Internal panda became available, skipping recovery")
        recovery_skipped = True
      else:
        previous_devnum = _tici_usb_attr("devnum") if is_tici() else None
        if recovery_actions == 2:
          cloudlog.info("No pandas found, putting internal panda into DFU")
          HARDWARE.recover_internal_panda()
        else:
          cloudlog.info("No pandas found, resetting internal panda")
          HARDWARE.reset_internal_panda()
        recovery_actions += 1
        time.sleep(3)  # wait to come back up
        wait_for_tici_panda_reenumeration(previous_devnum)

      wait_for_tici_panda_udev()

      # Flash all Pandas in DFU mode
      dfu_serials = list_dfu_pandas()
      if len(dfu_serials) > 0:
        previous_devnum = _tici_usb_attr("devnum") if is_tici() else None
        for serial in dfu_serials:
          cloudlog.info(f"Panda in DFU mode found, flashing recovery {serial}")
          PandaDFU(serial).recover()
        time.sleep(1)
        wait_for_tici_panda_reenumeration(previous_devnum)
        wait_for_tici_panda_udev()

      panda_serials = list_pandas()
      if len(panda_serials) == 0:
        no_internal_panda_count += 1
        continue

      cloudlog.info(f"{len(panda_serials)} panda(s) found, connecting - {panda_serials}")

      # Update and reserve the Rivian harness bridge before managing internal Pandas.
      bridge_serials = prepare_rivian_bridge(panda_serials)
      panda_serials = [serial for serial in panda_serials if serial not in bridge_serials]
      if len(panda_serials) == 0:
        no_internal_panda_count += 1
        continue

      # Flash pandas
      pandas: list[Panda] = []
      remote_start = get_remote_start_boots_comma(params)
      hkg_remote_start = get_hkg_remote_start_boots_comma(params)
      ignore_ignition_line = get_ignore_ignition_line(params)
      tesla_wake = get_tesla_wake_on_can(params)
      for serial in panda_serials:
        pandas.append(flash_panda(serial, remote_start, hkg_remote_start, ignore_ignition_line, tesla_wake))

      # Ensure internal panda is present if expected
      internal_pandas = [panda for panda in pandas if panda.is_internal()]
      if HARDWARE.has_internal_panda() and len(internal_pandas) == 0:
        cloudlog.error("Internal panda is missing, trying again")
        no_internal_panda_count += 1
        continue
      no_internal_panda_count = 0

      # sort pandas to have deterministic order
      # * the internal one is always first
      # * then sort by hardware type
      # * as a last resort, sort by serial number
      pandas.sort(key=lambda x: (not x.is_internal(), x.get_type(), x.get_usb_serial()))
      panda_serials = [p.get_usb_serial() for p in pandas]

      # log panda fw versions
      params.put("PandaSignatures", b','.join(p.get_signature() for p in pandas))

      for panda in pandas:
        # check health for lost heartbeat
        health = panda.health()
        if health["heartbeat_lost"]:
          params.put_bool("PandaHeartbeatLost", True)
          cloudlog.event("heartbeat lost", deviceState=health, serial=panda.get_usb_serial())
        if health["som_reset_triggered"]:
          params.put_bool("PandaSomResetTriggered", True)
          cloudlog.event("panda.som_reset_triggered", health=health, serial=panda.get_usb_serial())

        if first_run:
          # reset panda to ensure we're in a good state
          cloudlog.info(f"Resetting panda {panda.get_usb_serial()}")
          panda.reset(reconnect=True)

      for p in pandas:
        p.close()
    # TODO: wrap all panda exceptions in a base panda exception
    except (usb1.USBErrorNoDevice, usb1.USBErrorPipe):
      # a panda was disconnected while setting everything up. let's try again
      cloudlog.exception("Panda USB exception while setting up")
      continue
    except PandaProtocolMismatch:
      cloudlog.exception("pandad.protocol_mismatch")
      continue
    except Exception:
      cloudlog.exception("pandad.uncaught_exception")
      continue

    first_run = False

    # run pandad with all connected serials as arguments
    if remote_start or hkg_remote_start or ignore_ignition_line or tesla_wake:
      os.environ["BOARDD_SKIP_FW_CHECK"] = "1"
    else:
      os.environ.pop("BOARDD_SKIP_FW_CHECK", None)
    os.environ['MANAGER_DAEMON'] = 'pandad'
    process = subprocess.Popen(["./pandad", *panda_serials], cwd=os.path.join(BASEDIR, "selfdrive/pandad"))
    process.wait()


if __name__ == "__main__":
  main()
