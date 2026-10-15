
#include <lvgl.h>
#include <zmk/display/status_screen.h>
#include "assets/eye_center.h"

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    // Keep the working black background
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    // Eye image: rotate 180 degrees
    lv_obj_t *eye = lv_image_create(screen);
    lv_image_set_src(eye, &wolfie_eye_center);

    // Mirror original x=2, y=16 positioning
    // across the 160 x 68 screen
    lv_obj_set_pos(eye, 84, 6);
    lv_image_set_rotation(eye, 1800);

    // Keep working eye colour
    lv_obj_set_style_image_recolor(
        eye, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(
        eye, LV_OPA_COVER, 0);

    // Header: move to opposite corner
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "WOLFIE R");
    lv_obj_set_style_text_color(
        label, lv_color_black(), 0);

    lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, -4, -1);

    // Rotate text around its centre
    lv_obj_update_layout(label);
    lv_obj_set_style_transform_pivot_x(
        label, lv_obj_get_width(label) / 2, 0);
    lv_obj_set_style_transform_pivot_y(
        label, lv_obj_get_height(label) / 2, 0);
    lv_obj_set_style_transform_rotation(label, 1800, 0);

    return screen;
}
