
#include <lvgl.h>
#include <zmk/display/status_screen.h>
#include "assets/eye_center.h"

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    // Static trackball eye

    // Temporary test: white rectangle
    lv_obj_t *eye_test = lv_obj_create(screen);

    lv_obj_set_size(eye_test, 74, 46);
    lv_obj_align(eye_test, LV_ALIGN_TOP_LEFT, 2, 16);

    lv_obj_set_style_bg_color(eye_test, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(eye_test, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(eye_test, 0, 0);
    lv_obj_set_style_radius(eye_test, 0, 0);
    lv_obj_set_style_pad_all(eye_test, 0, 0);


    // Temporary header
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "WOLFIE R");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 4, 1);

    return screen;
}
