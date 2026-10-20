/*
 * Static Wolfie L 160x68 space scene, preserving the existing split and SPI.
 * Uses the user's chosen source image, with a readable WOLFIE L label.
 * Only local battery-status timer; no changes to input devices or roles.
 */
#include <lvgl.h>
#include <stdint.h>
#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zmk/battery.h>
#include "assets/wolfie_left_battery_glyphs.h"
#include "assets/wolfie_space_tiles.h"

static void add_white_scene_tile(lv_obj_t *screen, const lv_image_dsc_t *src,
                                  int32_t x, int32_t y)
{
    lv_obj_t *sprite = lv_image_create(screen);
    lv_image_set_src(sprite, src);
    lv_obj_set_pos(sprite, x, y);
    lv_obj_set_style_image_recolor(sprite, lv_color_white(), 0);
    lv_obj_set_style_image_recolor_opa(sprite, LV_OPA_COVER, 0);
}



/* Local battery reporting on the left ZMK BLE peripheral (not the right half's
 * percentage). No split BLE settings or keyboard scan configuration changed.
 *
 * The static scene bitmaps are already oriented correctly on the actual
 * display. Battery sprites must use those SAME coordinates, without the
 * extra 180-degree position flip. CONFIG_ZMK_DISPLAY_INVERT=y means the
 * LVGL white backing appears black, and black-recolored digits appear white.
 */
#define WOLFIE_DISPLAY_W 160
#define WOLFIE_DISPLAY_H 68
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

static lv_obj_t *wolfie_left_create_digit(lv_obj_t *screen,
                                           const lv_image_dsc_t *image,
                                           int physical_x, int physical_y)
{
    lv_obj_t *obj = lv_image_create(screen);
    lv_image_set_src(obj, image);
    /* Scene sprites and display coordinates are already upright.
     * Do not mirror the battery digits independently. */
    lv_obj_set_pos(obj, physical_x, physical_y);
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
    lv_obj_set_pos(backing, WOLFIE_BATT_BOX_X, WOLFIE_BATT_BOX_Y);
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
    wolfie_left_add_battery_status(screen);
    return screen;
}
