# Wolfie right nice!view — keep the eye visible but freeze during idle

Based on the **hardware-tested BT1 CPI switching** source from
`Wolfie-BT1-400CPI-Profile-Switch.zip`, which retains your five layer names,
animated eye, Bluetooth/battery/USB status, and runtime BT1=400/other=1000 CPI.

Only one file changes: `boards/shields/wolfie_niceview/custom_status_screen.c`.

## Behaviour

- Active: eye continues to drift; the status and battery remain visible.
- After the existing 60-second ZMK idle timeout: the eye freezes in its last
  location, **without blanking the screen**.
- On a keypress or PMW3610 movement: eye movement resumes on the next existing
  250 ms status timer tick, then on its own next scheduled animation tick.
- Deep sleep after 20 minutes: unchanged. Both halves wake as before.
- The existing combined layer/BT status timer continues to run; this patch
  saves the eye's motion redraw work, but does not fully stop LVGL processing.

IMPORTANT: Keep `CONFIG_ZMK_DISPLAY_BLANK_ON_IDLE=n` in the right shield
configuration. Setting it to `y` would blank the display instead of freezing
an image. No change to this configuration is included in the patch.

## GitHub Desktop

1. Create/keep a backup branch in `wolfie-niceview` from your current good build.
2. On main, merge the ZIP's `boards` directory into `wolfie-niceview`.
3. GitHub Desktop should show a change to just **one** C source file.
4. Commit/push, then run a fresh GitHub Actions build in
   `Wolfie-Split-Trackball-ZMK`.
5. Flash only the **right** 60s-idle/20m-deep-sleep optimized display build,
   `wolfie_split_right_sleep_20m.uf2`, from the NEW artifact.
6. Check moving eye, BT1=400 CPI, all five layer labels, and smooth tracking.
   Leave untouched for 75–90 seconds: the eye should stay visible but not move.
   Move the trackball or press a key and confirm animation resumes.

## Rollback

Reflash the last known-good right UF2, or restore the old right
`custom_status_screen.c` from your backup branch. Left firmware unchanged.

This is a source-only diagnostic update; a successful GitHub Actions build and
hardware test are still required before considering it proven.
