# Wolfie left nice!view: black-background / white-artwork update

This update keeps the same approved left space scene, but **inverts the tile data** so the actual nice!view should render with a **black background** and **white planet, stars, station and `WOLFIE L` text**.

Included files:

- `boards/shields/wolfie_leftview/assets/wolfie_space_tiles.h` (replaced)
- `boards/shields/wolfie_leftview/custom_status_screen.c` (included unchanged for completeness)

## Installing

1. Keep your current working UF2 backups.
2. Open **wolfie-niceview** in GitHub Desktop on `main`.
3. Extract this ZIP and copy its `boards` folder into the repository root, merging folders.
4. Confirm only the two files above changed. Commit as `Invert left space display to black background` and push.
5. In **Wolfie-Split-Trackball-ZMK**, run the GitHub Actions workflow on `main` so it fetches the updated module.
6. After the build succeeds, flash **only** `wolfie_split_left_display_test.uf2` to the left nice!nano.

This is a visual-only left-display change. The right eye, trackball, keymaps and wireless split configuration are unchanged.
