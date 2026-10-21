# Wolfie — per-profile CPI (right nice!view firmware)

## CPI mapping
- BT0: 1000 CPI
- **BT1: 400 CPI** (ZMK's second Bluetooth slot)
- BT2, BT3, BT4: 1000 CPI

This patch is built directly from your uploaded latest `custom_status_screen.c`.
It adds sensor CPI switching to the existing 250 ms display status timer;
it does not change the eye motion, combined status sprites, battery, USB,
keymap, BLE scheduling, or the left nice!view.

The sensor driver is not ready immediately after boot (Wolfie config adds an
extra 1000 ms power-up delay), so failed early requests are retried until the
sensor is initialized. When the Bluetooth profile changes, CPI should follow
within around 250 ms; the first change after boot can take longer.

### GitHub Desktop installation

1. Make sure you have backups of the working smooth-BLE firmware and dual displays.
2. Open **wolfie-niceview** on the branch currently used by your ZMK build
   (usually `main`).
3. Copy this ZIP's `boards` folder into the **wolfie-niceview** repository root,
   merging folders and replacing ONLY:
   `boards/shields/wolfie_niceview/custom_status_screen.c`
4. Review the single-file diff, commit `Switch PMW3610 CPI on BT1`, and push.
5. Open **Wolfie-Split-Trackball-ZMK**, run fresh GitHub Actions firmware build.
6. Flash ONLY the successful full-display, BLE-priority-4 right firmware target:
   `wolfie_split_right_bt75_sensor7_bleprio4_display.uf2`
   (If that artifact isn't generated, inspect your existing build.yaml first.)
7. Test BT0 at 1000 CPI, BT1 at 400 CPI, back to BT0/BT2 at 1000 CPI.
   Eye animation, layer names and battery should remain unchanged.

### Important implementation note
The Wolfieselket PMW3610 ALT driver exposes its CPI change via `sensor_attr_set`.
Its private `enum pmw3610_alt_attribute` starts at zero for `PMW3610_ALT_ATTR_CPI`.
The code uses that numeric value because the driver's `src/pmw3610.h` isn't an
exported include path of the nice!view module. If your driver fork changes its
enum values in future, this small adaptor must be updated accordingly.

### Build notes
This archive is a source patch, NOT a compiled UF2. It hasn't yet been tested
in your GitHub Actions toolchain or flashed on hardware. If it fails, send the
first compiler error; the old working firmware remains available to reflash.
