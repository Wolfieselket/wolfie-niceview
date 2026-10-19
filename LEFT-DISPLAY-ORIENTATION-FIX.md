# Wolfie left nice!view — upright `WOLFIE L` fix

The first left-screen test displayed `WOLFIE L` upside down but the 22 switches and split Bluetooth link still worked. This patch rotates the **existing 67×14 A1 sprite data by 180°**. It changes *only*:

`boards/shields/wolfie_leftview/assets/wolfie_l_upright.h`

The image dimensions, image descriptor, SPI1 configuration, custom status screen renderer, keyboard matrix, PMW3610 and both halves' split firmware are unchanged.

## Apply in GitHub Desktop
1. Open `wolfie-niceview`, select `main`. Preserve `working-static-eye`.
2. Extract this ZIP and copy its `boards` folder into your local `wolfie-niceview` repository, replacing the single matching header file.
3. Verify in **Changes** that only the header above is modified. Commit `Correct WOLFIE L orientation` and **Push origin**.
4. In `Wolfie-Split-Trackball-ZMK`, run GitHub Actions on `main` to fetch the updated module.
5. From the new successful run, flash **only** `wolfie_split_left_display_test.uf2` to the left nice!nano. Leave the right central firmware untouched; no settings reset is required.
6. Confirm text upright and left keys still working through the right central.

**Note**: Updating the external display module requires a new ZMK build even when the main keyboard repository has not changed. GitHub Actions may fetch a cached module revision if the manifest pins a commit; if the output doesn't change, inspect the manifest ref and build logs before assuming the patch failed.
