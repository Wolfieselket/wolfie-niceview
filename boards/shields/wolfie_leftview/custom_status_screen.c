/*
 * Cassini SXR-02-L: animated spacecraft passing through the existing
 * user-selected 160x68 space scene; battery and LINK remain visible.
 * New Saturn enclosure mounts the left LCD 180 degrees relative to Wolfie.
 * Rotate assets and mirror their positions; do not rotate the LVGL driver.
 * Keep Bluetooth peripheral, matrix and SPI unchanged.
 */
#include <lvgl.h>
#include <stdint.h>
#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zmk/battery.h>
#include <zmk/activity.h>
#include <zmk/split/bluetooth/peripheral.h>
#include "assets/wolfie_left_status_glyphs.h"
#include "assets/wolfie_left_battery_glyphs.h"
#include "assets/wolfie_space_tiles.h"
#include "assets/cassini_left_orbit.h"

#define WOLFIE_DISPLAY_W 160
#define WOLFIE_DISPLAY_H 68

/* Mirror positions to complement all pre-rotated A1 sprite pixels. */
static void saturn_left_set_rotated_pos(lv_obj_t *obj, int32_t x, int32_t y,
                                         int32_t w, int32_t h)
{
    lv_obj_set_pos(obj, WOLFIE_DISPLAY_W - x - w,
                       WOLFIE_DISPLAY_H - y - h);
}

static void add_white_scene_tile(lv_obj_t *screen, const lv_image_dsc_t *src,
                                  int32_t x, int32_t y)
{
    lv_obj_t *sprite = lv_image_create(screen);
    lv_image_set_src(sprite, src);
    saturn_left_set_rotated_pos(sprite, x, y, src->header.w, src->header.h);
    lv_obj_set_style_image_recolor(sprite, lv_color_white(), 0);
    lv_obj_set_style_image_recolor_opa(sprite, LV_OPA_COVER, 0);
}



/* Local battery reporting on the left ZMK BLE peripheral (not the right half's
 * percentage). No split BLE settings or keyboard scan configuration changed.
 *
 * Cassini left artwork and glyphs are pre-rotated 180 degrees; their
 * LVGL positions must be mirrored too, while keeping the existing colour
 * inversion, battery reporting, and split LINK logic unchanged.
 */
#define WOLFIE_BATT_BOX_X 2
#define WOLFIE_BATT_BOX_Y 2
#define WOLFIE_BATT_BOX_W 33
#define WOLFIE_BATT_BOX_H 13
#define WOLFIE_BATT_TEXT_X 4
#define WOLFIE_BATT_TEXT_Y 4
#define WOLFIE_BATT_UPDATE_MS 15000

static lv_obj_t *wolfie_left_battery_digits[4];
static lv_timer_t *wolfie_left_battery_timer;
static int16_t wolfie_left_last_charge = -1;
/* Accurate BLE link status from this peripheral to the right central. */
#define WOLFIE_LINK_X 127
#define WOLFIE_LINK_Y 2
#define WOLFIE_LINK_POLL_MS 1500
/* Lightweight 24-frame orbiter pauses whenever ZMK leaves ACTIVE state. */
static lv_obj_t *cassini_left_orbit;
static lv_timer_t *cassini_left_orbit_timer;
static uint8_t cassini_left_orbit_next_frame;
static bool cassini_left_orbit_paused_for_idle;

static void cassini_left_orbit_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    if (cassini_left_orbit == NULL) { return; }
    lv_image_set_src(cassini_left_orbit,
                     cassini_left_orbit_frames[cassini_left_orbit_next_frame]);
    cassini_left_orbit_next_frame =
        (uint8_t)((cassini_left_orbit_next_frame + 1U) % CASSINI_LEFT_ORBIT_FRAME_COUNT);
}

static lv_obj_t *wolfie_left_link_status;
static lv_timer_t *wolfie_left_link_timer;
static int8_t wolfie_left_last_link = -1;
static void wolfie_left_refresh_link(void)
{
    int8_t connected = zmk_split_bt_peripheral_is_connected() ? 1 : 0;
    if (wolfie_left_last_link != connected) {
        lv_image_set_src(wolfie_left_link_status,
                         connected ? &wolfie_left_status_link_up_img
                                   : &wolfie_left_status_link_down_img);
        wolfie_left_last_link = connected;
    }
}
static void wolfie_left_link_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    wolfie_left_refresh_link();
    /* Reuse the existing LINK poll to manage animation idle/wake state. */
    if (cassini_left_orbit_timer != NULL) {
        const bool should_pause = zmk_activity_get_state() != ZMK_ACTIVITY_ACTIVE;
        if (should_pause != cassini_left_orbit_paused_for_idle) {
            if (should_pause) {
                lv_timer_pause(cassini_left_orbit_timer);
            } else {
                lv_timer_resume(cassini_left_orbit_timer);
            }
            cassini_left_orbit_paused_for_idle = should_pause;
        }
    }
}

static lv_obj_t *wolfie_left_create_digit(lv_obj_t *screen,
                                           const lv_image_dsc_t *image,
                                           int physical_x, int physical_y)
{
    lv_obj_t *obj = lv_image_create(screen);
    lv_image_set_src(obj, image);
    /* Digit and LINK glyph pixels are pre-rotated in their asset files. */
    saturn_left_set_rotated_pos(obj, physical_x, physical_y,
                                 image->header.w, image->header.h);
    lv_obj_set_style_image_recolor(obj, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(obj, LV_OPA_COVER, 0);
    return obj;
}

static void wolfie_left_update_battery(void)
{
    uint8_t level = zmk_battery_state_of_charge();
    if (level > 100U) {
        level = 100U;
    }
    if (wolfie_left_last_charge == (int16_t)level) {
        return;
    }

    const uint8_t digits[3] = {
        level / 100U, (level / 10U) % 10U, level % 10U
    };
    for (uint8_t i = 0; i < 3U; ++i) {
        const bool visible = (i == 2U) ||
                             (i == 1U && level >= 10U) ||
                             (i == 0U && level == 100U);
        if (visible) {
            lv_image_set_src(wolfie_left_battery_digits[i], wolfie_digits[digits[i]]);
            lv_obj_remove_flag(wolfie_left_battery_digits[i], LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(wolfie_left_battery_digits[i], LV_OBJ_FLAG_HIDDEN);
        }
    }
    wolfie_left_last_charge = level;
}

static void wolfie_left_battery_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    wolfie_left_update_battery();
}

static void wolfie_left_screen_delete_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) != LV_EVENT_DELETE) {
        return;
    }
    if (wolfie_left_battery_timer != NULL) {
        lv_timer_delete(wolfie_left_battery_timer);
        wolfie_left_battery_timer = NULL;
    }
    if (wolfie_left_link_timer != NULL) {
        lv_timer_delete(wolfie_left_link_timer);
        wolfie_left_link_timer = NULL;
    }
    if (cassini_left_orbit_timer != NULL) {
        lv_timer_delete(cassini_left_orbit_timer);
        cassini_left_orbit_timer = NULL;
    }
    cassini_left_orbit = NULL;
    cassini_left_orbit_next_frame = 0U;
    cassini_left_orbit_paused_for_idle = false;
    wolfie_left_link_status = NULL;
    wolfie_left_last_link = -1;
    for (uint8_t i = 0; i < 4U; ++i) {
        wolfie_left_battery_digits[i] = NULL;
    }
    wolfie_left_last_charge = -1;
}

static void wolfie_left_add_battery_status(lv_obj_t *screen)
{
    /* The small backing masks a few stars so battery digits stay readable. */
    lv_obj_t *backing = lv_obj_create(screen);
    lv_obj_set_size(backing, WOLFIE_BATT_BOX_W, WOLFIE_BATT_BOX_H);
    saturn_left_set_rotated_pos(backing, WOLFIE_BATT_BOX_X, WOLFIE_BATT_BOX_Y,
                                 WOLFIE_BATT_BOX_W, WOLFIE_BATT_BOX_H);
    lv_obj_set_style_bg_color(backing, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(backing, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(backing, 0, 0);
    lv_obj_set_style_outline_width(backing, 0, 0);
    lv_obj_set_style_shadow_width(backing, 0, 0);
    lv_obj_set_style_pad_all(backing, 0, 0);
    lv_obj_set_style_radius(backing, 0, 0);
    lv_obj_remove_flag(backing, LV_OBJ_FLAG_SCROLLABLE);

    for (uint8_t i = 0; i < 3U; ++i) {
        wolfie_left_battery_digits[i] = wolfie_left_create_digit(
            screen, &wolfie_digit_0_img,
            WOLFIE_BATT_TEXT_X + 7 * i, WOLFIE_BATT_TEXT_Y);
    }
    wolfie_left_battery_digits[3] = wolfie_left_create_digit(
        screen, &wolfie_percent_img,
        WOLFIE_BATT_TEXT_X + 21, WOLFIE_BATT_TEXT_Y);

    wolfie_left_last_charge = -1;
    wolfie_left_update_battery();
    lv_obj_add_event_cb(screen, wolfie_left_screen_delete_cb, LV_EVENT_DELETE, NULL);
    wolfie_left_battery_timer = lv_timer_create(
        wolfie_left_battery_timer_cb, WOLFIE_BATT_UPDATE_MS, NULL);
}

lv_obj_t *zmk_display_status_screen(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_radius(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    add_white_scene_tile(screen, &wolfie_space_tile_0, 0, 0);
    add_white_scene_tile(screen, &wolfie_space_tile_1, 80, 0);
    add_white_scene_tile(screen, &wolfie_space_tile_2, 0, 34);
    add_white_scene_tile(screen, &wolfie_space_tile_3, 80, 34);

    /* The orbiter follows the same 180-degree transform as the starfield. */
    cassini_left_orbit = lv_image_create(screen);
    lv_image_set_src(cassini_left_orbit, cassini_left_orbit_frames[0]);
    saturn_left_set_rotated_pos(cassini_left_orbit,
                                 CASSINI_LEFT_ORBIT_X, CASSINI_LEFT_ORBIT_Y,
                                 cassini_left_orbit_frames[0]->header.w,
                                 cassini_left_orbit_frames[0]->header.h);
    lv_obj_set_style_image_recolor(cassini_left_orbit, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(cassini_left_orbit, LV_OPA_COVER, 0);
    cassini_left_orbit_next_frame = 1U;
    cassini_left_orbit_paused_for_idle = false;
    cassini_left_orbit_timer = lv_timer_create(
        cassini_left_orbit_timer_cb, CASSINI_LEFT_ORBIT_FRAME_MS, NULL);

    wolfie_left_add_battery_status(screen);
    /* Small monochrome masking rectangle keeps the starfield behind LINK legible.
     * The left screen's known-good inversion means white here appears black. */
    lv_obj_t *link_backing = lv_obj_create(screen);
    lv_obj_set_size(link_backing, 31, 11);
    saturn_left_set_rotated_pos(link_backing, WOLFIE_LINK_X, WOLFIE_LINK_Y, 31, 11);
    lv_obj_set_style_bg_color(link_backing, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(link_backing, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(link_backing, 0, 0);
    lv_obj_set_style_pad_all(link_backing, 0, 0);
    lv_obj_set_style_radius(link_backing, 0, 0);
    lv_obj_remove_flag(link_backing, LV_OBJ_FLAG_SCROLLABLE);
    wolfie_left_link_status = wolfie_left_create_digit(
        screen, &wolfie_left_status_link_down_img, WOLFIE_LINK_X, WOLFIE_LINK_Y + 1);
    wolfie_left_last_link = -1;
    wolfie_left_refresh_link();
    wolfie_left_link_timer = lv_timer_create(wolfie_left_link_timer_cb, WOLFIE_LINK_POLL_MS, NULL);
    return screen;
}
