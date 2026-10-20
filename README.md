# Wolfie right: eye-safe combined Bluetooth + layer diagnostic

This update is based on the right-hand **eye + layer-only firmware** confirmed
working on hardware. That version's eye creation, bitmaps, movement timers,
label, battery and USB status are preserved.

The previous Bluetooth test added an extra LVGL image object, after which the
eye disappeared. This package deliberately adds **no additional display
objects or timers**. Instead it replaces the working layer-only image with a
slightly taller combined A1 bitmap containing both layer and Bluetooth labels.
Four pre-rotated 34 x 25 sprites cover QWERTY/FUNCTION x BT+/BT-.

The layer updates every 250 ms, and Bluetooth profile connection checks run
every 1000 ms. The indicator describes the active BLE host profile, not RSSI.
The display code still calls ZMK's `zmk_ble_active_profile_is_connected()`.

## Install

1. Use the wolfie-niceview **main** branch; keep all backup branches intact.
2. Copy only the `boards` folder from this ZIP onto your local wolfie-niceview
   repo, merging/overwriting. Two files are added/changed:
   - `boards/shields/wolfie_niceview/custom_status_screen.c` (modified)
   - `boards/shields/wolfie_niceview/assets/wolfie_combined_status_glyphs.h` (new)
3. Commit and push using GitHub Desktop.
4. Run a NEW workflow in Wolfie-Split-Trackball-ZMK and download the NEW artifact.
5. Flash **only** `wolfie_split_right_central.uf2` to the right half.
6. Test eye animation first, then QWERTY/FUNCTION speed and BT+ status.

Do not flash the left half or reset Bluetooth pairings.

If the eye still disappears, restore the last confirmed eye+layer-only renderer
from `Wolfie-right-layer-only-diagnostic.zip` and keep BT status disabled
pending further debugging. This patch is source/asset verified but not
hardware tested.
