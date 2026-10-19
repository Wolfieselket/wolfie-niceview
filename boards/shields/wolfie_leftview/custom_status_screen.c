/* Left-side nice!view static diagnostic screen.
 * Intentionally no layer or Bluetooth widget, no timers, no animation.
 * Independent of the right-side wolfie_niceview module.
 */
#include <lvgl.h>
#include "assets/wolfie_l_upright.h"

lv_obj_t *zmk_display_status_screen(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_radius(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *label = lv_image_create(screen);
    lv_image_set_src(label, &wolfie_l_img);
    lv_obj_set_pos(label, (160 - WOLFIE_L_WIDTH) / 2,
                   (68 - WOLFIE_L_HEIGHT) / 2);
    lv_obj_set_style_image_recolor(label, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(label, LV_OPA_COVER, 0);
    return screen;
}
