# Upstream StarPilot selector fixture (2a12dbd0); never used to flash firmware.
def get_selected_firmware_name(app_fn: str, remote_start: bool, hkg_remote_start: bool, ignore_ignition_line: bool,
                               tesla_wake: bool = False) -> str:
  if tesla_wake and (remote_start or hkg_remote_start):
    raise ValueError("Tesla wake firmware cannot be combined with remote-start firmware")
  if not remote_start and not hkg_remote_start and not ignore_ignition_line and not tesla_wake:
    return app_fn

  name_parts = ["panda_h7" if app_fn == "panda_h7.bin.signed" else "panda"]
  if tesla_wake:
    name_parts.extend(["tesla", "wake"])
  elif hkg_remote_start:
    name_parts.extend(["hkg", "remote"])
  elif remote_start:
    name_parts.append("remote")
  if ignore_ignition_line:
    name_parts.append("can_ignition_only")
  return "_".join(name_parts) + ".bin.signed"
