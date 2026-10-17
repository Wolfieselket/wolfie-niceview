#include <lvgl.h>
#include <zmk/display/status_screen.h>
#include "assets/eye_center.h"

/* Restore the original, unrotated static eye. No animation/rotation.
 * Logical white background + black ink with ZMK_DISPLAY_INVERT=y
 * produces a physically black screen with white ink on the nice!view.
 */
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_radius(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *eye = lv_image_create(screen);
    lv_image_set_src(eye, &wolfie_eye_center);
    lv_obj_set_pos(eye, (160 - EYE_WIDTH) / 2, 3);
    lv_obj_set_style_image_recolor(eye, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(eye, LV_OPA_COVER, 0);

    /* Small diagnostic label makes it easy to tell if LVGL is rendering. */
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "WOLFIE R");
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -1);
    return screen;
}
