# Wolfie display photo follow-up — focused two-file test

The hardware photographs show:
- Right: BT profile, layer, battery and `WOLFIE R` text visible, **eye absent**.
- Left: Space art, battery and `WOLFIE L` upright, **LINK glyph inverted**.

## Changes

1. `boards/shields/wolfie_niceview/custom_status_screen.c`:
   - Preserves the existing eye and status widgets and their positions.
   - Moves the existing eye and `WOLFIE R` label to the foreground after constructing the new status widgets.
   - Explicitly invalidates the eye region during its existing slow movement cycle.
   - **This is a targeted diagnostic for the missing eye, not a proven root-cause fix.**
2. `boards/shields/wolfie_leftview/assets/wolfie_left_status_glyphs.h`:
   - Un-rotates the `LINK+` and `LINK-` image data so they follow the *same tested image orientation* as the working left battery numbers.
   - Does not alter any other images or C functions.

## Installation

1. In GitHub Desktop open **wolfie-niceview**, branch `main`; leave the working display backup branches untouched.
2. Extract this ZIP and merge its `boards` folder into the repository root.
3. Confirm that **only the two files listed above** changed.
4. Commit as `Correct left link glyphs and test right eye redraw`, then push.
5. In **Wolfie-Split-Trackball-ZMK** run a fresh `main` workflow. Wait for both split display builds to succeed.
6. Flash **only `wolfie_split_right_central.uf2`** to the right first and check the animated eye, BT, layer and battery. Keep a copy of your earlier successful UF2.
7. If the right eye is restored and the keyboard/trackball work, flash **`wolfie_split_left_display_test.uf2`** to the left and check LINK orientation.

Do not flash the older `wolfie_split_left_peripheral` file: it lacks the left display. Do not reset Bluetooth settings or alter wiring. If the right eye is still missing, report that result before we modify further. The new code has been reviewed but has **not** been compiled in GitHub Actions or verified on hardware.
