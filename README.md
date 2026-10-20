# Wolfie right-side layer-only test

This is an incremental diagnostic based on the **last hardware-verified working right eye renderer**. It restores the working eye, motion timer, battery percentage, USB indicator and `WOLFIE R`, and adds only one layer-name widget (`QWERTY` / `FUNCTION`).

There is **no Bluetooth-status widget**, no new SPI configuration, no changes to the left display, no keymap changes and no split pairing reset.

Included files:

- `boards/shields/wolfie_niceview/custom_status_screen.c` — based on the confirmed working eye-and-battery renderer, plus only the layer status logic
- `boards/shields/wolfie_niceview/assets/wolfie_layer_only_glyphs.h` — two small 34 × 9 monochrome sprites

## Install

1. Keep copies of the two last-known-working split UF2 files and preserve your backup branches.
2. Open `wolfie-niceview` in GitHub Desktop on **main**.
3. Extract this ZIP, copy its `boards` directory to the repository root, merging directories and replacing the existing `custom_status_screen.c`.
4. Check that only the right `custom_status_screen.c` is modified and the new `wolfie_layer_only_glyphs.h` is added. Commit as `Test right layer status with animated eye` and push.
5. Open `Wolfie-Split-Trackball-ZMK` → Actions → run the workflow on **main**. Wait for successful completion and download the artifact from this new run.
6. Flash **only** `wolfie_split_right_central.uf2` to the right nice!nano. The left remains on its current firmware.
7. Confirm the animated eye appears and moves; verify the battery reading, trackball and normal typing. Then hold and release the Function-layer thumb key and verify `QWERTY` → `FUNCTION` → `QWERTY`.

If the eye disappears or unintended typing starts, disconnect and restore your saved last-known-working central UF2. Do not reset Bluetooth pairings; they are unrelated to this test.

**This patch has been structurally checked but has not been compiled or tested on hardware.**
