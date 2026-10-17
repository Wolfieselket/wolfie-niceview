# Upright static-eye diagnostic — small-sprite method

Copy the **contents** of this ZIP into your local `wolfie-niceview` repository on `main`, retaining its other existing files, then commit and push. This changes only `custom_status_screen.c` and adds two sprite headers. It deliberately does not change the display's SPI configuration, the nice!view driver, the key matrix, or the PMW3610.

The actual physical display is rotated 180 degrees compared with LVGL. The existing successful eye asset is rotated pixel by pixel into a **74×46, A1** sprite. `WOLFIE R` is rendered to a **64×13, A1** sprite, rotated the same way. Their logical positions are inverted so that, on the physically upside-down display, the eye is above the text. No LVGL transforms or full-screen image buffers are used.

## Build and flash

1. Commit and push the module update to `Wolfieselket/wolfie-niceview`.
2. In `Wolfieselket/Wolfie-Split-Trackball-ZMK`, **re-run** your existing `wolfie_display_only_diagnostic` GitHub Actions build. This should fetch the updated module from its `main` branch.
3. Download and flash **only** the `wolfie_display_only_diagnostic` UF2. The real keyboard matrix and trackball should remain inactive during this test.
4. Check that the eye and `WOLFIE R` appear upright and don't flicker.

If the screen is blank, restore the last confirmed working `wolfie-niceview` commit and rerun the diagnostic build. Do not alter the keyboard or sensor wiring.

No local ZMK compilation or hardware test has been done for this patch; GitHub Actions will validate compilation.
