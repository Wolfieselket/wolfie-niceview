# Wolfie left battery percentage — orientation correction

This is a minimal fix for **upside-down battery digits** on the left nice!view. The confirmed-working space scene and `WOLFIE L` lettering are not changed. All ten numeral glyphs and the percent sign have been rotated 180 degrees within their existing 7x9 A1 bitmap data, with identical descriptors and dimensions.

## Installation

1. In GitHub Desktop, open **wolfie-niceview**, select **main** (leave `working-left-space-static` untouched).
2. Extract the ZIP and copy the **boards** folder into that repository's root, merging/replacing the matching file.
3. The only changed tracked file should be `boards/shields/wolfie_leftview/assets/wolfie_left_battery_glyphs.h`.
4. Commit as `Correct left battery digits orientation` and push.
5. In **Wolfie-Split-Trackball-ZMK**, run GitHub Actions on **main** to pull the module update.
6. After a successful build, flash only `wolfie_split_left_display_test.uf2` onto the **left** nice!nano.

No pairing reset required, and no changes to the right-hand firmware, PMW3610, keys or BLE split configuration.

## Check

The left screen should still show the planet, stars and `WOLFIE L` in their current orientation, with the battery percentage now readable normally. It still refreshes approximately every 15 seconds. This patch has not been compiled or flashed locally; if unexpected issues arise, restore the last working left UF2.
