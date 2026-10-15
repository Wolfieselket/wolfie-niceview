
#include <lvgl.h>
#include <zmk/display/status_screen.h>
#include "assets/eye_center.h"


lv_obj_t *zmk_display_status_screen(void) {
    // Request 180-degree rotation
    lv_display_set_rotation(
        lv_display_get_default(),
        LV_DISPLAY_ROTATION_180
    );

    lv_obj_t *screen = lv_obj_create(NULL);


    // Screen background
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    // Static trackball eye
    lv_obj_t *eye = lv_image_create(screen);
    lv_image_set_src(eye, &wolfie_eye_center);
    lv_obj_align(eye, LV_ALIGN_TOP_LEFT, 2, 16);
    lv_obj_set_style_image_recolor(eye, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(eye, LV_OPA_COVER, 0);

    // Temporary header
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "WOLFIE R");
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 4, 1);

    return screen;
}
