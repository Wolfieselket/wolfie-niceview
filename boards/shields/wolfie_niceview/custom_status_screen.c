/*
 * Wolfie nice!view — animation stage 1 + battery percentage and USB power status.
 *
 * Preserve the two proven, small 1-bit sprite assets, the physical 180-degree
 * pre-rotation, and all display hardware configuration.
 *
 * For the first live animation test, the eye gently drifts left and right;
 * the WOLFIE R label remains stationary. This tests scheduled LVGL updates,
 * not pupil movement or trackball-reactive behaviour yet.
 */
#include <lvgl.h>
#include <zephyr/kernel.h>
#include <zmk/battery.h>
#include <zmk/keymap.h>
#include <zmk/ble.h>
#include "assets/wolfie_combined_status_glyphs.h"
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
#include <zmk/usb.h>
#endif
#include "assets/wolfie_battery_glyphs.h"
#include <stdint.h>
#include "assets/wolfie_eye_upright.h"
#include "assets/wolfie_label_upright.h"

#define WOLFIE_LCD_WIDTH 160
#define WOLFIE_EYE_BASE_X ((WOLFIE_LCD_WIDTH - UPRIGHT_EYE_WIDTH) / 2)

/*
 * The display is mounted 180 degrees from LVGL coordinates, so a positive
 * logical X offset makes the eye move LEFT when viewed on the keyboard.
 * Stages: left, centre, right, centre; initial position is centre.
 */
static const int8_t eye_x_offsets[] = { +4, 0, -4, 0 };
static const uint16_t eye_hold_ms[] = { 850, 1400, 850, 2500 };

static lv_obj_t *animated_eye;
static lv_timer_t *eye_motion_timer;
static uint8_t animation_phase;

/* Right-side battery display, physically upright thanks to pre-rotated A1 glyphs. */
#define WOLFIE_BATTERY_X 119  /* physical display left edge of the 4-character reading */
#define WOLFIE_BATTERY_Y 21
#define WOLFIE_USB_X 128
#define WOLFIE_USB_Y 37
#define WOLFIE_LCD_HEIGHT 68

static lv_obj_t *battery_glyph[4];
static lv_obj_t *usb_glyph;
static lv_timer_t *battery_refresh_timer;
static int16_t previous_level = -1;
static int8_t previous_usb = -1;

/* One status object shared by layer + Bluetooth indicator.
 * No extra LVGL widgets or timers beyond the hardware-tested layer version.
 * Layer is polled at 250ms; active Bluetooth profile at 1000ms.
 */
#define WOLFIE_STATUS_X 3
#define WOLFIE_STATUS_Y 5
#define WOLFIE_LAYER_REFRESH_MS 250
#define WOLFIE_BT_CHECK_EVERY_TICKS 4U
static lv_obj_t *wolfie_layer_status;
static lv_timer_t *wolfie_layer_timer;
static int8_t wolfie_last_layer = -1;
static int8_t wolfie_last_connected = -1;
static int8_t wolfie_current_connected = 0;
static uint8_t wolfie_bt_ticks = 0U;

static const lv_image_dsc_t *wolfie_status_sprite(uint8_t layer, uint8_t connected)
{
    if (connected) {
        return layer ? &wolfie_status_on_function_img : &wolfie_status_on_qwerty_img;
    }
    return layer ? &wolfie_status_off_function_img : &wolfie_status_off_qwerty_img;
}

static void wolfie_refresh_layer_status(bool force_bluetooth)
{
    if (wolfie_layer_status == NULL) { return; }
    const int8_t layer = zmk_keymap_highest_layer_active() == 0 ? 0 : 1;
    if (force_bluetooth || (++wolfie_bt_ticks >= WOLFIE_BT_CHECK_EVERY_TICKS)) {
        wolfie_bt_ticks = 0U;
        wolfie_current_connected = zmk_ble_active_profile_is_connected() ? 1 : 0;
    }
    if (layer != wolfie_last_layer || wolfie_current_connected != wolfie_last_connected) {
        lv_image_set_src(wolfie_layer_status,
                         wolfie_status_sprite(layer, wolfie_current_connected));
        wolfie_last_layer = layer;
        wolfie_last_connected = wolfie_current_connected;
    }
}

static void wolfie_layer_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    wolfie_refresh_layer_status(false);
}


/* Objects are positioned in logical LVGL coordinates. Hardware is mounted 180°.
 * Thus we reflect the physical positions, while the image data is itself rotated.
 * Never rotate the complete display with LVGL; that broke our earlier tests.
 */
static lv_obj_t *wolfie_make_glyph(lv_obj_t *screen, const lv_image_dsc_t *img,
                                   int phys_x, int phys_y)
{
    lv_obj_t *obj = lv_image_create(screen);
    lv_image_set_src(obj, img);
    lv_obj_set_pos(obj, WOLFIE_LCD_WIDTH - phys_x - img->header.w,
                   WOLFIE_LCD_HEIGHT - phys_y - img->header.h);
    lv_obj_set_style_image_recolor(obj, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(obj, LV_OPA_COVER, 0);
    return obj;
}

static void wolfie_refresh_battery(void)
{
    /* ZMK's own battery reading is a voltage-based percentage estimate. */
    uint8_t level = zmk_battery_state_of_charge();
    if (level > 100U) { level = 100U; }

    if (previous_level != (int16_t)level) {
        const uint8_t values[3] = {level / 100U, (level / 10U) % 10U, level % 10U};
        for (uint8_t i = 0; i < 3U; ++i) {
            bool visible = (i == 2U) || (i == 1U && level >= 10U) ||
                           (i == 0U && level == 100U);
            if (visible) {
                lv_image_set_src(battery_glyph[i], wolfie_digits[values[i]]);
                lv_obj_remove_flag(battery_glyph[i], LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_add_flag(battery_glyph[i], LV_OBJ_FLAG_HIDDEN);
            }
        }
        previous_level = level;
    }

    /* "USB" means external USB power is present, NOT confirmed charging. */
    bool usb_present = false;
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    usb_present = zmk_usb_is_powered();
#endif
    if (previous_usb != (int8_t)usb_present) {
        if (usb_present) {
            lv_obj_remove_flag(usb_glyph, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(usb_glyph, LV_OBJ_FLAG_HIDDEN);
        }
        previous_usb = (int8_t)usb_present;
    }
}

static void wolfie_battery_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    wolfie_refresh_battery();
}

static void wolfie_eye_timer_cb(lv_timer_t *timer)
{
    if (animated_eye == NULL) {
        return;
    }

    uint8_t phase = animation_phase;
    lv_obj_set_x(animated_eye, WOLFIE_EYE_BASE_X + eye_x_offsets[phase]);
    lv_timer_set_period(timer, eye_hold_ms[phase]);

    animation_phase = (uint8_t)((phase + 1U) % 4U);
}

/* Release the timer if ZMK/LVGL ever replaces the status screen. */
static void wolfie_status_screen_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_DELETE) {
        if (eye_motion_timer != NULL) {
            lv_timer_delete(eye_motion_timer);
            eye_motion_timer = NULL;
        }
        if (battery_refresh_timer != NULL) {
            lv_timer_delete(battery_refresh_timer);
            battery_refresh_timer = NULL;
        }
        if (wolfie_layer_timer != NULL) {
            lv_timer_delete(wolfie_layer_timer);
            wolfie_layer_timer = NULL;
        }
        wolfie_layer_status = NULL;
        animated_eye = NULL;
        usb_glyph = NULL;
        for (uint8_t i = 0; i < 4U; ++i) { battery_glyph[i] = NULL; }
    }
}

lv_obj_t *zmk_display_status_screen(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_radius(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* Original working, pre-rotated 74x46 eye at logical y=19. */
    lv_obj_t *eye = lv_image_create(screen);
    lv_image_set_src(eye, &wolfie_eye_upright);
    lv_obj_set_pos(eye, WOLFIE_EYE_BASE_X, 19);
    lv_obj_set_style_image_recolor(eye, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(eye, LV_OPA_COVER, 0);

    /* Original working pre-rotated label — deliberately remains fixed. */
    lv_obj_t *label = lv_image_create(screen);
    lv_image_set_src(label, &wolfie_label_upright);
    lv_obj_set_pos(label, (WOLFIE_LCD_WIDTH - UPRIGHT_LABEL_WIDTH) / 2, 2);
    lv_obj_set_style_image_recolor(label, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(label, LV_OPA_COVER, 0);

    /* Start centred, wait before the first movement, and repeat slowly. */
    animated_eye = eye;
    animation_phase = 0;
    lv_obj_add_event_cb(screen, wolfie_status_screen_event_cb, LV_EVENT_DELETE, NULL);
    eye_motion_timer = lv_timer_create(wolfie_eye_timer_cb, 1600, NULL);

    /* 3 digits + percent, in physical reading order. Use hidden leading zeroes. */
    for (uint8_t i = 0; i < 3U; ++i) {
        battery_glyph[i] = wolfie_make_glyph(screen, &wolfie_digit_0_img,
                                               WOLFIE_BATTERY_X + 7 * i,
                                               WOLFIE_BATTERY_Y);
    }
    battery_glyph[3] = wolfie_make_glyph(screen, &wolfie_percent_img,
                                          WOLFIE_BATTERY_X + 21, WOLFIE_BATTERY_Y);
    usb_glyph = wolfie_make_glyph(screen, &wolfie_usb_img,
                                   WOLFIE_USB_X, WOLFIE_USB_Y);
    previous_level = -1;
    previous_usb = -1;
    wolfie_refresh_battery();
    battery_refresh_timer = lv_timer_create(wolfie_battery_timer_cb, 15000, NULL);

    /* Replace the old layer-only sprite with one combined status image.
     * It occupies the same leftmost region and never overlaps the eye. */
    wolfie_layer_status = wolfie_make_glyph(screen,
                                           &wolfie_status_off_qwerty_img,
                                           WOLFIE_STATUS_X, WOLFIE_STATUS_Y);
    wolfie_last_layer = -1;
    wolfie_last_connected = -1;
    wolfie_current_connected = 0;
    wolfie_bt_ticks = 0U;
    wolfie_refresh_layer_status(true);
    wolfie_layer_timer = lv_timer_create(wolfie_layer_timer_cb,
                                        WOLFIE_LAYER_REFRESH_MS, NULL);

    return screen;
}
