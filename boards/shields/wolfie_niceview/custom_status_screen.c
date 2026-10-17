#include <lvgl.h>
#include "assets/wolfie_eye_upright.h"
#include "assets/wolfie_label_upright.h"

/*
 * Display-only diagnostic: firmware scans no real matrix keys or trackball.
 * Hardware is physically mounted 180 degrees relative to LVGL coordinates.
 * Do not call lv_display_set_rotation() or use style/object transforms.
 * These two small sprites are already rotated in their A1 pixel data.
 */
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_radius(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* LVGL Y=19 becomes physical top Y=3 after the 180-degree mounting. */
    lv_obj_t *eye = lv_image_create(screen);
    lv_image_set_src(eye, &wolfie_eye_upright);
    lv_obj_set_pos(eye, (160 - UPRIGHT_EYE_WIDTH) / 2, 19);
    lv_obj_set_style_image_recolor(eye, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(eye, LV_OPA_COVER, 0);

    /* Put pre-rotated label at LVGL top -> physically below the eye. */
    lv_obj_t *label = lv_image_create(screen);
    lv_image_set_src(label, &wolfie_label_upright);
    lv_obj_set_pos(label, (160 - UPRIGHT_LABEL_WIDTH) / 2, 2);
    lv_obj_set_style_image_recolor(label, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(label, LV_OPA_COVER, 0);

    return screen;
}
