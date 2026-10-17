# Wolfie nice!view — animated eye + battery status

This is a **two-file update to the `wolfie-niceview` repository**. It does not change the trackball, keyboard matrix, SPI configuration, display orientation settings, or existing eye/label sprites.

## Installation (GitHub Desktop)

1. Open `Wolfieselket/wolfie-niceview`, branch **main**.
2. Extract this ZIP. Copy the **boards** directory into the root of the local repository, merging folders and replacing only `custom_status_screen.c`. One new asset will be added: `assets/wolfie_battery_glyphs.h`.
3. Check GitHub Desktop's **Changes** tab: there should be **one modified file and one added file**.
4. Commit `Add battery percentage and USB power indicator` and **Push origin**.
5. In `Wolfie-Split-Trackball-ZMK`, re-run the GitHub Actions build on **main** and flash ONLY the previously proven combined `wolfie_keys_eye_pmw_qwerty` firmware artifact from this new run.
6. Leave the `working-static-eye` and `working-complete-right-half` backup branches unchanged.

## Display behaviour

* The upright animated eye and `WOLFIE R` continue unchanged.
* The right margin shows the battery **percentage** (e.g. `85%`). This is ZMK's estimated state of charge, **not a precision fuel gauge**.
* `USB` appears below it **when external USB power is detected**. This indicates connection to power, **not necessarily that charging current is flowing**. The nice!nano's charger does not report charging state through this display API.
* The level and USB indicator update every **15 seconds** while the display is active. A reading of 0% during the first seconds after boot may reflect initial sampling rather than an empty cell.
* All glyphs are small immutable 1-bit sprites pre-rotated 180 degrees, like the successfully tested eye. No LVGL rotation or full-screen image is used.

## Testing

With the battery connected and USB disconnected, check the percentage is visible and `USB` is hidden. Plug in USB and wait up to 15 seconds; `USB` should appear. Unplug and wait up to 15 seconds; it should disappear. Test QWERTY, AltGr, the trackball and eye animation. Stop and restore the working firmware if uncontrolled keystrokes return.

**Build check:** This update has been structurally generated and is not locally built against the entire ZMK/Zephyr toolchain. The GitHub Actions build is the first integration/compile check. Report the first compiler error if it fails.

Note: `CONFIG_ZMK_BATTERY_REPORTING` is normally enabled on Bluetooth builds. If the firmware shows a persistent 0% despite a charged battery, we should check the effective Kconfig and board battery ADC sensor before changing the graphics.
