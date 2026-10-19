# Wolfie left nice!view — battery percentage

This patch adds the **left half's own battery percentage** in the upper-left
corner of the working black-background / white-artwork `WOLFIE L` space scene.
The original planet, station, stars, text, tile bitmaps and split firmware
configuration remain unchanged.

Files included:
- `boards/shields/wolfie_leftview/custom_status_screen.c` (modified)
- `boards/shields/wolfie_leftview/assets/wolfie_left_battery_glyphs.h` (new)

## Installation in GitHub Desktop

1. Ensure `working-left-space-static` in `wolfie-niceview` is saved.
2. Open `wolfie-niceview`, switch to **main**.
3. Extract this ZIP; copy the included **boards** folder into the repository root.
4. In GitHub Desktop confirm only the files listed above changed. Commit and Push.
5. Go to `Wolfie-Split-Trackball-ZMK` -> Actions -> Run workflow on **main**.
6. After successful build, flash ONLY `wolfie_split_left_display_test.uf2`
   onto the **left** nice!nano. Leave right half firmware unchanged.

## Test

- With USB unplugged and both halves on their batteries, the left display
  should show its own percentage at the **upper-left**, e.g. `63%`.
- It refreshes every **15 seconds**; ZMK gives a voltage-derived estimate.
- Check all left keys still type, the split reconnects, the right eye keeps
  moving and the PMW3610 trackball still works.
- If the left graphic is blank, inverted or misbehaves, reflash the saved
  working left UF2. No Bluetooth settings reset should be necessary.
- USB-connected battery estimates can temporarily appear higher; the reading
  does **not** confirm charging is complete.

This update has been checked for asset dimensions and file structure, but
has NOT been compiled in GitHub Actions or tested on your hardware.
