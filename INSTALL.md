# Wolfie status overlays: BLE host, active layer, peripheral link

Install ONLY to **wolfie-niceview** (display module repository), on branch `main`.
Keep `working-both-displays-battery` intact. This is not a replacement for any keyboard GPIO or build files.

## Files updated

- `boards/shields/wolfie_niceview/custom_status_screen.c`
- `boards/shields/wolfie_niceview/assets/wolfie_right_status_glyphs.h` (new)
- `boards/shields/wolfie_leftview/custom_status_screen.c`
- `boards/shields/wolfie_leftview/assets/wolfie_left_status_glyphs.h` (new)

## Changes

Right central: BT1+ / BT1- (active host profile, connected/disconnected; profiles BT1..BT5), plus QWERTY or FUNCTION (not an RSSI signal meter). The eye animation, right battery gauge, USB and label are preserved.

Left peripheral: LINK+ / LINK- (BLE connection to the central). Static space art and battery percentage are preserved.

The status text uses the same pre-rotated 1-bit glyph convention as the working battery percentage sprites. Only changes the displayed bitmap when status changes (polled around 1 to 1.5 seconds). No physical wiring, keymap or split configuration changes.

## Steps

1. Extract and copy `boards/` to your local `wolfie-niceview` repository, merging the two shield subfolders.
2. Review four file changes in GitHub Desktop; commit as `Add Bluetooth, split-link and layer status to displays` then push.
3. Run a **fresh** workflow in `Wolfie-Split-Trackball-ZMK` on branch `main` so it fetches the module update.
4. Build must succeed before flashing; this patch has not been ZMK-compiled here.
5. Flash `wolfie_split_right_central.uf2` on the right, and verify BT host status and FUNCTION on the right. The right firmware was changed only in its display code.
6. If stable, flash `wolfie_split_left_display_test.uf2` on the left and verify LINK+, keyboard, right trackball and battery.
7. If failures occur, restore both archived previous split UF2 files without resetting Bluetooth bonds; the backup branches are untouched.

Expected: On the right, BT1+ for host profile 1 connected; BT1- when disconnected, and QWERTY -> FUNCTION while the momentary layer is held. On the left, LINK+ when joined to right central, LINK- if disconnected. No RSSI bars are implied.

Small pre-rotated status text has not yet been checked on hardware. If it is upside down, do not change SPI/device-tree settings; only the new status bitmap assets require adjustment.
