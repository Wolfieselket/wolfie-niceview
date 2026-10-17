/*
 * Wolfie nice!view — animation stage 1.
 *
 * Preserve the two proven, small 1-bit sprite assets, the physical 180-degree
 * pre-rotation, and all display hardware configuration.
 *
 * For the first live animation test, the eye gently drifts left and right;
 * the WOLFIE R label remains stationary. This tests scheduled LVGL updates,
 * not pupil movement or trackball-reactive behaviour yet.
 */
#include <lvgl.h>
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
        animated_eye = NULL;
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

    return screen;
}
