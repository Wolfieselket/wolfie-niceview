/*
 * Static Wolfie L 160x68 space scene, preserving the existing split and SPI.
 * Uses the user's chosen source image, with a readable WOLFIE L label.
 * No timers or changes to input devices or firmware roles.
 */
#include <lvgl.h>
#include "assets/wolfie_space_tiles.h"

static void add_white_scene_tile(lv_obj_t *screen, const lv_image_dsc_t *src,
                                  int32_t x, int32_t y)
{
    lv_obj_t *sprite = lv_image_create(screen);
    lv_image_set_src(sprite, src);
    lv_obj_set_pos(sprite, x, y);
    lv_obj_set_style_image_recolor(sprite, lv_color_white(), 0);
    lv_obj_set_style_image_recolor_opa(sprite, LV_OPA_COVER, 0);
}

lv_obj_t *zmk_display_status_screen(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_set_style_radius(screen, 0, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    add_white_scene_tile(screen, &wolfie_space_tile_0, 0, 0);
    add_white_scene_tile(screen, &wolfie_space_tile_1, 80, 0);
    add_white_scene_tile(screen, &wolfie_space_tile_2, 0, 34);
    add_white_scene_tile(screen, &wolfie_space_tile_3, 80, 34);
    return screen;
}
