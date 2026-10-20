# Wolfie right nice!view — five-layer label fix

## Scope
This patch modifies only **two files** in the `wolfie-niceview` repository:

- `boards/shields/wolfie_niceview/custom_status_screen.c`
- `boards/shields/wolfie_niceview/assets/wolfie_combined_status_glyphs.h`

It fixes the old logic which converted *every* nonzero layer to FUNCTION. The
five current Wolfie layer indices now display:

| ZMK layer | Display label | Note |
| --- | --- | --- |
| 0 | QWERTY | Original known-good sprite |
| 1 | SYMBOLS | Short form of `Sym, Num, Arr` |
| 2 | FUNCTION | Original known-good sprite, short form of `Functions` |
| 3 | MOUSE | New sprite |
| 4 | GAMING | New sprite |
| 5+ | LAYER | Safe generic fallback for additional layers |

**The names are fixed images, not automatically taken from Keymap Editor.**
If you rename/reorder layers in the future, update these image labels or the
C switch table. Automatically displaying arbitrary layer names would require a
separate, more invasive runtime text-rendering change.

## Preserved
- The existing QWERTY / FUNCTION / BT+ / BT- pixel data are byte-for-byte unchanged.
- Original eye and label sprites, animation timers, rotation, LVGL object count,
  battery percentage, USB power indication, and BLE status logic are untouched.
- The additional sprites are pre-rotated A1 34×25 and use the same **single LVGL
  image object** for combined layer + Bluetooth indication.

## Installation

1. Back up the working right-side firmware `.uf2` and mark the current successful
   firmware and display repository commits before changing any files.
2. In `Wolfieselket/wolfie-niceview`, replace **both** files at the paths above.
3. Commit the change, e.g. `Display five Wolfie keymap layers`.
4. Trigger a **fresh** build in `Wolfieselket/Wolfie-Split-Trackball-ZMK`.
   Ensure your main repository fetches this new `wolfie-niceview` commit.
   If your ZMK west manifest pins a revision, update that revision deliberately.
5. If GitHub Actions compiles successfully, flash the **right-central** firmware.
   You do not need to reflash the left peripheral for this right-display-only patch.
6. Test QWERTY, Symbols, Functions, Mouse, Gaming, and both BLE states.
   If a build or runtime test fails, restore both backed-up files (or revert the
   display-repository commit) and flash your last-known-good `.uf2`.

## Verification performed
- Data arrays are 125 bytes each (A1, 34×25, stride 5).
- Every BT portion matches the existing corresponding connected/disconnected
  sprite byte-for-byte.
- The original C file was changed only in the layer selector and its 0/1
  reduction; everything else was preserved.
- A full Zephyr/ZMK compilation and hardware test were **not** run here.
