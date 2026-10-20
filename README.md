# Wolfie left battery display — orientation and placement fix

This update corrects **both issues visible in the hardware photograph**:
- The battery percentage appears upside-down.
- The percentage appears at the bottom-right instead of the intended top-left.

Unlike previous attempts, the fix updates the **positions in `custom_status_screen.c`**
*and* provides explicitly upright 7x9 1-bit number/percent sprites in
`assets/wolfie_left_battery_glyphs.h`. These use the same unrotated coordinates
as the successfully tested `WOLFIE L` space scene.

All four approved space-scene tiles are unchanged. The LVGL black/white recolor
remains compatible with the display's existing inversion setting.

## Installation

1. Leave your `working-left-space-static` backup branch untouched.
2. In GitHub Desktop, select `main` in **wolfie-niceview**.
3. Extract this ZIP and copy `boards` into the local repository, merging folders.
4. Confirm TWO files changed:
   - `boards/shields/wolfie_leftview/custom_status_screen.c`
   - `boards/shields/wolfie_leftview/assets/wolfie_left_battery_glyphs.h`
5. Commit and push: `Correct left battery placement and orientation`.
6. Run **Wolfie-Split-Trackball-ZMK** Actions on `main` and wait for a successful build.
7. Download the **new** firmware artifact and flash only
   `wolfie_split_left_display_test.uf2` onto the left controller.

No right-hand, keyboard, Bluetooth or PMW3610 firmware changes are included.
The update still requires GitHub Actions compilation and hardware verification.

If the battery text is still wrong, share a fresh photo of the screen AND
confirm the build run time; older artifacts can contain earlier glyph versions.
