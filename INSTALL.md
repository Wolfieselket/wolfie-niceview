# Wolfie Right — fast layer status and compact Bluetooth indicator

Built **directly from the hardware-tested `Wolfie-right-layer-only-diagnostic.zip`** (eye animation and layer status working).

Changes:

- Layer name refresh: **1200 ms → 250 ms**. Actual key responsiveness is unchanged.
- One new small `BT+` / `BT-` indicator on the right at the physical upper-left, above the layer label.
- `BT+` means the active Bluetooth host profile is connected. `BT-` means it is not. This first conservative version **does not yet show the profile number or signal strength**.
- BLE checks every **1000 ms** (one check per four layer timer callbacks); redraws only when state changes.
- The eye animation, static WOLFIE R label, battery percentage/USB display, PMW3610, keymap, and left-display source are unchanged.

## Install

1. Keep your saved known-working UF2 files. Keep the backup branches unchanged.
2. Open `wolfie-niceview` in GitHub Desktop on `main`.
3. Extract this ZIP and copy the included `boards` folder into the repository root, merging folders and replacing files where prompted.
4. Verify exactly **one modified** source file (`wolfie_niceview/custom_status_screen.c`) and **one new** header (`wolfie_niceview/assets/wolfie_bt_compact_glyphs.h`).
5. Commit `Add compact BT indicator and faster layer display`, then Push origin.
6. Run the workflow in `Wolfie-Split-Trackball-ZMK` on `main`, then download the freshly completed artifact.
7. Flash **only** `wolfie_split_right_central.uf2` to the right half. No Bluetooth settings reset or left flash is necessary.

## Verify

- Animated eye remains visible and moving.
- Layer changes QWERTY → FUNCTION when held, returns on release; display latency should be at most roughly 250 ms plus screen update.
- `BT+` appears when the right half is paired/connected to the computer; `BT-` when disconnected. Allow up to a second after a connection change.
- Typing, trackball and split communication still function normally.

This update is **not locally compiled**. GitHub Actions must complete successfully before flashing.
If the eye disappears again, restore the last working right-side UF2. Do not clear Bluetooth pairings.
