# Wolfie LEFT nice!view — upright LINK status only

This is a LEFT-SIDE ONLY update. It does **not** contain any right display files.
Do not change or reflash the tested, working right half.

## Files

- `boards/shields/wolfie_leftview/custom_status_screen.c`: Keeps the original working static space scene and local battery percentage; adds the split-connection label if it is missing from the current source.
- `boards/shields/wolfie_leftview/assets/wolfie_left_status_glyphs.h`: Correct upright/unmirrored A1 glyph data for `LINK+` and `LINK-`, in the same convention as the **working** left battery digits.

The previous upside-down LINK came from sprites that had been rotated 180 degrees before use. The corrected glyphs are already upright. The actual scene and battery sprites are unmodified.

## Install

1. Open `wolfie-niceview` in GitHub Desktop, on `main`. Keep your backup branches.
2. Extract the ZIP, and merge the `boards` folder into your local `wolfie-niceview` repository.
3. Confirm that **only the two LEFT files above** change. Some may show as unchanged if already committed. The right-half display files must not change.
4. Commit changes as `Fix left LINK status orientation` and push.
5. Run a **new** GitHub Actions build in `Wolfie-Split-Trackball-ZMK`. Make sure the workflow is using the latest `wolfie-niceview` version, not an old pinned revision.
6. Download the **fresh** build artifact and flash only `wolfie_split_left_display_test.uf2` to the LEFT nice!nano.
7. Confirm the space artwork, left battery percentage, and now-upright LINK status all display correctly.

Important: If GitHub Desktop reports there are **no changes**, the corrected left code is already in `wolfie-niceview`. In that case there is nothing to commit: run a fresh firmware build and flash the new LEFT UF2. A repository change does not update hardware until you flash it.

Not yet compiled or verified on hardware. No wiring, Bluetooth pairing or keymap changes required.
