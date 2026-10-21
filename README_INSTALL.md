# Wolfie right nice!view — orbiting planet (first hardware integration test)

## Contents

This ZIP contains TWO files to put into your existing `Wolfieselket/wolfie-niceview` repository:

```
boards/shields/wolfie_niceview/custom_status_screen.c
boards/shields/wolfie_niceview/assets/wolfie_orbit_animation.h
```

The updated C file replaces only the eye asset/object and its timer callback with the approved 48-frame planet. The status screen's original positioning, mounted 180-degree rotation convention, and five working layer-status sprites are preserved. No changes to left-hand firmware, CPI logic, BT settings, battery code, idle or deep sleep configuration are required.

## Install via GitHub website

1. Save your current *working* right-hand UF2 and note the working Git commit, so you can revert quickly.
2. Open your repository `https://github.com/Wolfieselket/wolfie-niceview`.
3. Go to `boards/shields/wolfie_niceview/assets/` and **upload** the new `wolfie_orbit_animation.h` included here. It must be in the assets folder next to `wolfie_label_upright.h`.
4. Open `boards/shields/wolfie_niceview/custom_status_screen.c`, choose Edit, replace the file content with the included `custom_status_screen.c`, and commit the changes.
5. Trigger a fresh ZMK GitHub Actions build from your `Wolfie-Split-Trackball-ZMK` repository. Check that its module reference resolves to this updated `wolfie-niceview` commit; if pinned to a specific older commit, update the pin deliberately.
6. After a successful build, flash **only the right-hand central half** with its appropriate UF2.

## Test checklist

- Planet appears upright in the former eye position, with stationary ring, rotating planet texture and orbiting moon.
- `WOLFIE R` stays stationary.
- QWERTY, SYMBOLS, FUNCTION, MOUSE, and GAMING all display correctly.
- Connected/disconnected Bluetooth status and battery percentage are unaffected.
- BT1 still uses 400 CPI and BT0/2/3/4 still use 1000 CPI.
- After 60 seconds idle, the planet should stop animating without disappearing (as with your current eye). Verify this on actual hardware.
- After 20 minutes deep sleep, the right and left halves wake and reconnect as before.

## Specifications

- 48 frames, 74x46 pixels each, LVGL `LV_COLOR_FORMAT_A1` bitmaps.
- 180-degree pre-rotated image data, 10-byte stride per row, 460 bytes per frame.
- Frame period 85ms (11.76 frames/second), 4.08 second repeat.
- Frame bitmap storage 22,080 bytes total plus descriptors/pointers in firmware flash; no full animation allocated in RAM.
- One LVGL image object and one frame timer, replacing the eye object and timer.
- Exact idle/sleep behavior and image refresh rate have **not yet been hardware verified** for this new animation. Battery usage will be higher while awake than the previous slow-moving eye due to frequent screen refreshes.

## Roll back

Revert this commit in the display module (restoring the old `custom_status_screen.c`), rebuild, and reflash your known-working right-hand UF2. The new header can remain unused or be deleted; it has no effect unless included.

## Validation performed

- Confirmed the header contains 48 complete A1 frame arrays, 48 image descriptors, and matching references.
- Confirmed the 48-frame 160x68 GIF preview has the intended size.
- Performed a GCC syntax-only compile using lightweight LVGL/ZMK header stubs for the updated C and animation header.
- A real ZMK/Zephyr GitHub Actions build and physical nice!view test have **not** yet been run.
