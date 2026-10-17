# Static eye recovery test

This package is for **Wolfieselket/wolfie-niceview**, not the main keyboard repository.

- Restores `assets/eye_center.h` (74 x 46) instead of the experimental `assets/full_screen_180.h`.
- Removes rotation and includes a visible `WOLFIE R` diagnostic label.
- Leaves display overlay, wiring, PMW3610 configuration and keyboard keymap unchanged.
- Not compiled or hardware-tested; inspect GitHub Actions before flashing.

## Install
Use GitHub Desktop to clone/open `wolfie-niceview` (main branch), copy these files over the existing repository (excluding any `.git` directory), commit and push. Then in `Wolfie-Split-Trackball-ZMK`, Actions > Build PMW3610 Test Firmware > Run workflow, choose `main` and build; this causes `west` to fetch the latest `wolfie-niceview` revision. Choose the artifact ending `_eye`. The known-good keyboard-only firmware and `working-keyboard-trackball` branch should be left unchanged.

If both image and label remain blank after a successful display build, the next investigation is whether the Sharp display is electrically initialised on SPI1, whether the CS line and display power are correct, and whether LVGL status-screen registration is active. Do not alter matrix or sensor wiring.
