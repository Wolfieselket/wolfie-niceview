# Wolfie right nice!view — eye isolation test

## Why this build

The eye was confirmed working before the Bluetooth/layer status widgets were added. The first status update displayed the statuses but the eye vanished. A later foreground/invalidation patch did not restore it. This package restores the **exact previously successful eye + battery + USB renderer**, with no other firmware configuration changed, to isolate the problem.

Changes:
- **Modified:** `boards/shields/wolfie_niceview/custom_status_screen.c` only.
- **Temporarily absent:** the recently added BT-profile and QWERTY/Function status widgets, so we can test the original eye renderer.
- **Preserved:** original animated eye and `WOLFIE R`, right battery percentage/USB, left space artwork, left LINK state, wireless split, keymaps, trackball, and batteries.

## Installation

1. Open **wolfie-niceview** in GitHub Desktop on `main` (leave backup branches untouched).
2. Extract this ZIP into the **root of wolfie-niceview**, merging the `boards` folder. Only `boards/shields/wolfie_niceview/custom_status_screen.c` should show as modified. The status-glyph header may remain in the repository; it is harmless and unused by this test.
3. Commit `Diagnose missing right eye: restore working renderer` and push.
4. In **Wolfie-Split-Trackball-ZMK**, rerun GitHub Actions on `main`, so it fetches the updated `wolfie-niceview` module. Avoid reflashing an older artifact.
5. Download the **new** artifact from this new run, and flash ONLY `wolfie_split_right_central.uf2` on the **right** controller.
6. Confirm whether the eye and battery percentage return while the trackball, both keyboards, and the left display still work.

No Bluetooth settings reset, wiring change or left firmware flash is required. Save the currently compiled UF2 before overwriting.

### Interpretation

- **Eye returns:** the newly added right Bluetooth/layer status rendering causes the regression; we will reintroduce one status widget at a time and identify the specific interaction.
- **Eye stays absent:** we need to check that the right UF2 was compiled from this revision / exact module commit or inspect your on-device display build; there is no reason to tweak the eye bitmap yet.
