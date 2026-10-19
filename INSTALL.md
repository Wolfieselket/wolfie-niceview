# Wolfie left nice!view: approved static artwork

This update uses **the exact space-scene image supplied in the conversation** as its source, preserving the cratered moon, stars, shooting star, space-station arc and `WOLFIE L` label. The image was reduced to 160 × 68 pixels in 1-bit, and the existing readable `WOLFIE L` glyph was used in place of the tiny text from the large image.

Only these two firmware files are included:

- `boards/shields/wolfie_leftview/assets/wolfie_space_tiles.h` (new or replaced)
- `boards/shields/wolfie_leftview/custom_status_screen.c` (modified)

## Installing

1. Keep your **working-wireless-split** firmware backup and saved working left/right UF2 files.
2. Open **wolfie-niceview** in GitHub Desktop on `main`.
3. Extract this ZIP and copy its `boards` folder into that repository's root, merging existing directories.
4. In GitHub Desktop, review that only the two files above changed. Commit `Use approved space scene on left display` and push.
5. Open **Wolfie-Split-Trackball-ZMK** → Actions → run the workflow on `main` so it fetches the updated module.
6. After a successful build, flash **only** `wolfie_split_left_display_test.uf2` onto the left nice!nano.
7. Test the display's orientation, text readability and continued wireless key input. The right half should not require flashing or pairing again.

The artwork has been packaged into four small 80 × 34 monochrome A1 tiles, not one fullscreen image, to avoid our earlier full-screen rendering failure. The bitmap has **not** been tested with GitHub Actions or hardware yet.

If the display is black or shows unexpected behaviour, restore the saved known-working left UF2, without changing Bluetooth bonds.

The image is static. Animation can be added later after the hardware test succeeds.
