# Wolfie nice!view 180-degree bitmap diagnostic

Updates the `wolfie-niceview` module only. Uses the already working static display-only diagnostic build in `Wolfie-Split-Trackball-ZMK`.

- Precomposed eye + WOLFIE R into one 160x68 monochrome bitmap, then rotated the **pixels** 180 degrees.
- No LVGL or devicetree rotation; same display driver, SPI and ZMK settings.
- The display-only firmware intentionally does not scan keys or use the trackball.

## Installation

1. Open your local `wolfie-niceview` repo in GitHub Desktop, on `main`.
2. Copy the **contents** of this ZIP to its root, replacing files (do not create an extra enclosing folder).
3. Commit and push the change.
4. In `Wolfie-Split-Trackball-ZMK`, run the GitHub Actions workflow on `main` again so `west` fetches the latest display module. Download the **display-only diagnostic** firmware artifact and flash only that UF2.
5. Verify the eye and WOLFIE R are upright and do not flicker. Keyboard / trackball operation is disabled in this diagnostic.

Keep the original known-working keyboard/trackball firmware for recovery. These files are not compiled locally; successful build is still to be confirmed by GitHub Actions.
