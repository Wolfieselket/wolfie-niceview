#include <lvgl.h>
#include <zmk/display/status_screen.h>
#include "assets/upright_diagnostic_180.h"

/* Display-only diagnostic. The panel is installed upside down relative
 * to LVGL coordinates, so image AND text are rotated in the bitmap data.
 * Do not enable lv_display_set_rotation() or transform the LVGL screen.
 */
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_radius(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *image = lv_image_create(screen);
    lv_image_set_src(image, &wolfie_diag_upright);
    lv_obj_set_pos(image, 0, 0);
    lv_obj_set_style_image_recolor(image, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(image, LV_OPA_COVER, 0);
    return screen;
}
