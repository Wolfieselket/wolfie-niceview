#include <lvgl.h>
#include <zmk/display/status_screen.h>
#include "assets/full_screen_180.h"

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    /* Preserve the logical colour mapping that produced
     * a physically black background on your nice!view. */
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* Whole 160x68 bitmap has already been rotated 180 degrees;
     * do NOT request LVGL display or object rotation. */
    lv_obj_t *image = lv_image_create(screen);
    lv_image_set_src(image, &wolfie_right_rot180);
    lv_obj_set_pos(image, 0, 0);
    lv_obj_set_style_image_recolor(image, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(image, LV_OPA_COVER, 0);

    return screen;
}
