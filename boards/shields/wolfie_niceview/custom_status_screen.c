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
#include <zmk/ble.h>
#include <zmk/keymap.h>
#include "assets/wolfie_right_status_glyphs.h"
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
/* Compact status widgets at the physical left edge, clear of the animated eye. */
#define WOLFIE_BT_X 3
#define WOLFIE_BT_Y 5
#define WOLFIE_LAYER_X 3
#define WOLFIE_LAYER_Y 21
#define WOLFIE_STATUS_UPDATE_MS 1200
static lv_obj_t *wolfie_host_status;
static lv_obj_t *wolfie_layer_status;
static lv_timer_t *wolfie_status_timer;
static int8_t wolfie_previous_host_code = -1;
static int8_t wolfie_previous_layer = -1;
static const lv_image_dsc_t *const wolfie_host_images[5][2] = {
    {&wolfie_right_status_bt1_down_img, &wolfie_right_status_bt1_up_img},
    {&wolfie_right_status_bt2_down_img, &wolfie_right_status_bt2_up_img},
    {&wolfie_right_status_bt3_down_img, &wolfie_right_status_bt3_up_img},
    {&wolfie_right_status_bt4_down_img, &wolfie_right_status_bt4_up_img},
    {&wolfie_right_status_bt5_down_img, &wolfie_right_status_bt5_up_img},
};

static void wolfie_refresh_host_and_layer(void)
{
    /* + means a live Bluetooth host connection on the selected profile;
     * - means not connected. Neither is an RSSI measurement. */
    int profile = zmk_ble_active_profile_index();
    if (profile < 0 || profile >= 5) { profile = 0; }
    const int8_t connected = zmk_ble_active_profile_is_connected() ? 1 : 0;
    const int8_t host_code = (int8_t)(profile * 2 + connected);
    if (wolfie_previous_host_code != host_code) {
        lv_image_set_src(wolfie_host_status, wolfie_host_images[profile][connected]);
        wolfie_previous_host_code = host_code;
    }
    /* There are two user keymap layers: 0=QWERTY, 1=Function. */
    const int8_t layer = (zmk_keymap_highest_layer_active() == 0) ? 0 : 1;
    if (wolfie_previous_layer != layer) {
        lv_image_set_src(wolfie_layer_status,
                         (layer == 0) ? &wolfie_right_status_qwerty_img
                                      : &wolfie_right_status_function_img);
        wolfie_previous_layer = layer;
    }
}

static void wolfie_status_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    wolfie_refresh_host_and_layer();
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
    /* The image was missing after the new status sprites were introduced.
     * Force an eye redraw without changing SPI, artwork, or animation speed. */
    lv_obj_invalidate(animated_eye);
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
        if (wolfie_status_timer != NULL) {
            lv_timer_delete(wolfie_status_timer);
            wolfie_status_timer = NULL;
        }
        wolfie_host_status = NULL;
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
    wolfie_host_status = wolfie_make_glyph(screen, &wolfie_right_status_bt1_down_img,
                                           WOLFIE_BT_X, WOLFIE_BT_Y);
    wolfie_layer_status = wolfie_make_glyph(screen, &wolfie_right_status_qwerty_img,
                                            WOLFIE_LAYER_X, WOLFIE_LAYER_Y);
    wolfie_previous_host_code = -1;
    wolfie_previous_layer = -1;
    wolfie_refresh_host_and_layer();
    wolfie_status_timer = lv_timer_create(wolfie_status_timer_cb, WOLFIE_STATUS_UPDATE_MS, NULL);

    /* Re-assert the original eye and label as foreground objects after
     * creating the newly added status sprites. The status badges occupy
     * separate edge positions, so foreground order does not cover them. */
    lv_obj_move_foreground(eye);
    lv_obj_move_foreground(label);
    lv_obj_invalidate(eye);

    return screen;
}
