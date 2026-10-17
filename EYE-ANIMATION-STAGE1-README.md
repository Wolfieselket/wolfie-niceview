# Wolfie nice!view — Eye Animation, Stage 1

## Scope of this update

This is a **single-file patch** for the `wolfie-niceview` module, NOT a replacement for your `Wolfie-Split-Trackball-ZMK` firmware repository.

Changed:

- `boards/shields/wolfie_niceview/custom_status_screen.c`

Unchanged:

- `assets/wolfie_eye_upright.h` — known-working 74×46 pre-rotated image
- `assets/wolfie_label_upright.h` — known-working pre-rotated `WOLFIE R` text
- all nice!view SPI1 settings, build files, keyboard matrix, and PMW3610 driver

## Stage-1 animation

A slow **whole-eye horizontal drift** (not yet a moving pupil):

- Start centred for 1.6 seconds.
- Shift 4 physical pixels LEFT, hold ~0.85 seconds.
- Return to centre, hold ~1.4 seconds.
- Shift 4 physical pixels RIGHT, hold ~0.85 seconds.
- Return to centre, hold ~2.5 seconds.
- Repeat.

The `WOLFIE R` label stays stationary. Only an LVGL image object's X position changes, using an LVGL v9 timer. No display rotation, full-screen bitmaps, tracking events or changes to SPI pins.

## GitHub Desktop installation

1. Open **wolfie-niceview**, select `main` (the `working-static-eye` branch should remain untouched).
2. Extract this ZIP, and copy the **boards** directory into the root of your local `wolfie-niceview` checkout. Allow Windows to replace the one matching `custom_status_screen.c`.
3. In GitHub Desktop verify only one tracked source file changed, then commit as `Add slow eye drift animation` and **Push origin**.
4. Switch to `Wolfie-Split-Trackball-ZMK`, leave its main-branch firmware and all backup branches unchanged. Run its existing GitHub Actions workflow on `main` to fetch the updated `wolfie-niceview` module.
5. Only if the build succeeds, flash the **known working combined right-hand QWERTY + PMW3610 + eye** artifact (`wolfie_keys_eye_pmw_qwerty`, if still named that in your workflow).
6. Test eye drift, key stability, AltGr, and trackball motion. If there is a black screen or errant typing, disconnect and restore the last known-working UF2 from the backup branch.

## Limitations

- This is a **live-timer diagnostic animation** using the whole eye, not a pupil-gaze effect or blink yet. We can develop separate eye frames once this first animation is confirmed on hardware.
- The ZIP has been syntax-checked with an LVGL-API stub, but **has not been built with Zephyr/ZMK**. Let GitHub Actions validate it before flashing.
- A running timer will cause periodic nice!view updates and may reduce battery life; we can gate it on idle/sleep later.
