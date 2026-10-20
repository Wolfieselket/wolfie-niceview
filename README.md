# Wolfie left nice!view - physical battery text orientation fix

The previous battery glyph update accidentally made the numbers upright in LVGL screen coordinates, but the **left nice!view is mounted 180 degrees away from those coordinates**. This caused the battery digits to appear upside down even after the previous update.

This patch pre-rotates all ten digits and the percent symbol in the actual 7 x 9 bitmap data. It does not change the positions, percentage calculation, static space art, display driver, wireless split, or keyboard/trackball firmware.

Only one firmware source file is included:

`boards/shields/wolfie_leftview/assets/wolfie_left_battery_glyphs.h`

## Apply using GitHub Desktop

1. Open **wolfie-niceview** on `main` (leave `working-left-space-static` untouched).
2. Extract the ZIP and copy its `boards` folder into the repository, replacing the existing matching file.
3. In GitHub Desktop, confirm **only one header file** changed.
4. Commit with a message like `Pre-rotate left battery digits for mounted display` and **Push origin**.
5. In **Wolfie-Split-Trackball-ZMK**, manually run GitHub Actions workflow on `main` **after the module commit was pushed**.
6. Wait for all jobs to complete successfully. Download the artifact from **this newly completed run**, not a previously downloaded `firmware (N)` folder.
7. Flash **only** `wolfie_split_left_display_test.uf2` to the left nice!nano. Leave the right firmware and Bluetooth pairings unchanged.

**Validation:** after flashing, check whether a 1, 2, 3, 4, 5, 6, 7 or 9 reads upright; 0 and 8 look similar when rotated. If the digits are still upside down after this new build, send a photograph of the left screen showing the percentage and the GitHub Actions run number; we will verify which version was built and whether its asset is actually being used.

Rollback: flash your saved known-working left split display UF2 and/or return to the backup display-module commit.
