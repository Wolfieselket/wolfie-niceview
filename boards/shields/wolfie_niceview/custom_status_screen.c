/*
 * Wolfie right nice!view — 48-frame orbiting planet animation.
 *
 * Only the original eye graphic and its LVGL timer are replaced.
 * Existing five-layer status, Bluetooth, battery/USB rendering, 180-degree
 * physical rotation, and external ZMK idle/deep-sleep settings stay as-is.
 * CPI switches by selected BT profile: BT1=400, others=1000.
 */
#include <lvgl.h>
#include <zephyr/kernel.h>
#if IS_ENABLED(CONFIG_PMW3610_ALT)
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#endif
#include <zmk/battery.h>
#include <zmk/keymap.h>
#include <zmk/ble.h>
#include <zmk/activity.h>
#include "assets/wolfie_combined_status_glyphs.h"
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
#include <zmk/usb.h>
#endif
#include "assets/wolfie_battery_glyphs.h"
#include <stdint.h>
#include "assets/wolfie_orbit_animation.h"
#include "assets/wolfie_label_upright.h"

#define WOLFIE_LCD_WIDTH 160
#define WOLFIE_LCD_HEIGHT 68
/* 74x46 matches the former eye; preserve its LVGL position exactly. */
#define WOLFIE_ORBIT_WIDTH 74
#define WOLFIE_ORBIT_HEIGHT 46
#define WOLFIE_ORBIT_FRAME_COUNT 48U
#define WOLFIE_ORBIT_FRAME_MS 85U
#define WOLFIE_PLANET_BASE_X ((WOLFIE_LCD_WIDTH - WOLFIE_ORBIT_WIDTH) / 2)
#define WOLFIE_PLANET_BASE_Y 19

static lv_obj_t *animated_planet;
static lv_timer_t *planet_motion_timer;
static uint8_t planet_next_frame;
static bool planet_paused_for_idle;

/* Right-side battery display, physically upright thanks to pre-rotated A1 glyphs. */
#define WOLFIE_BATTERY_X 119  /* physical display left edge of the 4-character reading */
#define WOLFIE_BATTERY_Y 21
#define WOLFIE_USB_X 128
#define WOLFIE_USB_Y 37

static lv_obj_t *battery_glyph[4];
static lv_obj_t *usb_glyph;
static lv_timer_t *battery_refresh_timer;
static int16_t previous_level = -1;
static int8_t previous_usb = -1;

/* One status object shared by layer + Bluetooth indicator.
 * No additional LVGL widgets/timers beyond the original eye layout.
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

/*
 * Restore profile-aware CPI without adding a new LVGL timer.
 * BT profile index 1 = 400 CPI; every other profile = 1000 CPI.
 * Default device-tree CPI remains 1000 for startup.
 */
#if IS_ENABLED(CONFIG_PMW3610_ALT)
#define WOLFIE_CPI_BT1 400U
#define WOLFIE_CPI_DEFAULT 1000U
/* PMW3610_ALT_ATTR_CPI is enum value 0 in the installed PMW3610 ALT driver.
 * Its enum currently lives in the driver's internal src/pmw3610.h, not a
 * public header. Re-check this value if updating that driver module.
 */
#define WOLFIE_PMW3610_ATTR_CPI ((enum sensor_attribute)0)
static uint16_t wolfie_applied_cpi;

static void wolfie_refresh_profile_cpi(void)
{
    const struct device *sensor = DEVICE_DT_GET(DT_NODELABEL(trackball));
    if (!device_is_ready(sensor)) { return; }

    const uint16_t target = zmk_ble_active_profile_index() == 1
                                ? WOLFIE_CPI_BT1 : WOLFIE_CPI_DEFAULT;
    if (target == wolfie_applied_cpi) { return; }

    const struct sensor_value value = { .val1 = (int32_t)target, .val2 = 0 };
    /* Async PMW initialization can return -EBUSY; retry on the next tick. */
    if (sensor_attr_set(sensor, SENSOR_CHAN_ALL, WOLFIE_PMW3610_ATTR_CPI,
                        &value) == 0) {
        wolfie_applied_cpi = target;
    }
}
#endif

/* Show all five tested Wolfie keymap layer labels. */
static const lv_image_dsc_t *wolfie_status_sprite(uint8_t layer, uint8_t connected)
{
    switch (layer) {
    case 0:
        return connected ? &wolfie_status_on_qwerty_img : &wolfie_status_off_qwerty_img;
    case 1:
        return connected ? &wolfie_status_on_symbols_img : &wolfie_status_off_symbols_img;
    case 2:
        return connected ? &wolfie_status_on_function_img : &wolfie_status_off_function_img;
    case 3:
        return connected ? &wolfie_status_on_mouse_img : &wolfie_status_off_mouse_img;
    case 4:
        return connected ? &wolfie_status_on_gaming_img : &wolfie_status_off_gaming_img;
    default:
        return connected ? &wolfie_status_on_layer_img : &wolfie_status_off_layer_img;
    }
}

static void wolfie_refresh_layer_status(bool force_bluetooth)
{
    if (wolfie_layer_status == NULL) { return; }
    const uint8_t layer = zmk_keymap_highest_layer_active();
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
    /* Suspend only the orbit animation when ZMK is idle. */
    if (planet_motion_timer != NULL) {
        const bool should_pause = zmk_activity_get_state() != ZMK_ACTIVITY_ACTIVE;
        if (should_pause != planet_paused_for_idle) {
            if (should_pause) {
                lv_timer_pause(planet_motion_timer);
            } else {
                lv_timer_resume(planet_motion_timer);
            }
            planet_paused_for_idle = should_pause;
        }
    }
#if IS_ENABLED(CONFIG_PMW3610_ALT)
    wolfie_refresh_profile_cpi();
#endif
    wolfie_refresh_layer_status(false);
}

/* Hardware is mounted 180 degrees. Reflect LVGL positions and use
 * pre-rotated A1 image bytes: do NOT rotate the entire LVGL display. */
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
    /* ZMK's battery reading is a voltage-based percentage estimate. */
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

/* The first frame is set at creation; timer advances through all 48 frames.
 * Keeping the former single LVGL timer preserves current idle/wake handling.
 */
static void wolfie_planet_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    if (animated_planet == NULL) {
        return;
    }
    lv_image_set_src(animated_planet, wolfie_orbit_frames[planet_next_frame]);
    planet_next_frame = (uint8_t)((planet_next_frame + 1U) % WOLFIE_ORBIT_FRAME_COUNT);
}

/* Release timers if ZMK/LVGL ever replaces the status screen. */
static void wolfie_status_screen_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_DELETE) {
        if (planet_motion_timer != NULL) {
            lv_timer_delete(planet_motion_timer);
            planet_motion_timer = NULL;
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
        animated_planet = NULL;
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

    /* Replace only the original pre-rotated 74x46 eye at logical y=19. */
    lv_obj_t *planet = lv_image_create(screen);
    lv_image_set_src(planet, wolfie_orbit_frames[0]);
    lv_obj_set_pos(planet, WOLFIE_PLANET_BASE_X, WOLFIE_PLANET_BASE_Y);
    lv_obj_set_style_image_recolor(planet, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(planet, LV_OPA_COVER, 0);

    /* Original pre-rotated label — deliberately remains fixed. */
    lv_obj_t *label = lv_image_create(screen);
    lv_image_set_src(label, &wolfie_label_upright);
    lv_obj_set_pos(label, (WOLFIE_LCD_WIDTH - UPRIGHT_LABEL_WIDTH) / 2, 2);
    lv_obj_set_style_image_recolor(label, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(label, LV_OPA_COVER, 0);

    animated_planet = planet;
    planet_next_frame = 1U;
    planet_paused_for_idle = false;
    lv_obj_add_event_cb(screen, wolfie_status_screen_event_cb, LV_EVENT_DELETE, NULL);
    planet_motion_timer = lv_timer_create(wolfie_planet_timer_cb,
                                           WOLFIE_ORBIT_FRAME_MS, NULL);

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

    /* Same single combined layer+BT object as current firmware. */
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
