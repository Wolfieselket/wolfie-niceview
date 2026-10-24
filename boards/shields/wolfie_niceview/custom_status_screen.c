
    /* "USB" means external USB power is present, NOT confirmed charging. */
    bool usb_present = false;
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    usb_present = zmk_usb_is_powered();
#endif
    if (previous_usb != (int8_t)usb_present) {
        if (usb_present) {
            lv_obj_remove_flag(usb_glyph, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(usb_glyph, LV_OBJ_FLAG_HIDDEN);
        }
        previous_usb = (int8_t)usb_present;
    }
}

static void wolfie_battery_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    wolfie_refresh_battery();
}

/* The first frame is set at creation; timer advances through all 48 frames.
 * Keeping the former single LVGL timer preserves current idle/wake handling.
 */
static void wolfie_planet_timer_cb(lv_timer_t *timer)
{
    ARG_UNUSED(timer);
    if (animated_planet == NULL) {
        return;
    }
    lv_image_set_src(animated_planet, wolfie_orbit_frames[planet_next_frame]);
    planet_next_frame = (uint8_t)((planet_next_frame + 1U) % WOLFIE_ORBIT_FRAME_COUNT);
}

/* Release timers if ZMK/LVGL ever replaces the status screen. */
static void wolfie_status_screen_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_DELETE) {
        if (planet_motion_timer != NULL) {
            lv_timer_delete(planet_motion_timer);
            planet_motion_timer = NULL;
        }
        if (battery_refresh_timer != NULL) {
            lv_timer_delete(battery_refresh_timer);
            battery_refresh_timer = NULL;
        }
        if (wolfie_layer_timer != NULL) {
            lv_timer_delete(wolfie_layer_timer);
            wolfie_layer_timer = NULL;
        }
        wolfie_layer_status = NULL;
        animated_planet = NULL;
        usb_glyph = NULL;
        for (uint8_t i = 0; i < 4U; ++i) { battery_glyph[i] = NULL; }
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

    /* Replace only the original pre-rotated 74x46 eye at logical y=19. */
    lv_obj_t *planet = lv_image_create(screen);
    lv_image_set_src(planet, wolfie_orbit_frames[0]);
    lv_obj_set_pos(planet, WOLFIE_PLANET_BASE_X, WOLFIE_PLANET_BASE_Y);
    lv_obj_set_style_image_recolor(planet, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(planet, LV_OPA_COVER, 0);

    /* Original pre-rotated label — deliberately remains fixed. */
    lv_obj_t *label = lv_image_create(screen);
    lv_image_set_src(label, &wolfie_label_upright);
    lv_obj_set_pos(label, (WOLFIE_LCD_WIDTH - UPRIGHT_LABEL_WIDTH) / 2, 2);
    lv_obj_set_style_image_recolor(label, lv_color_black(), 0);
    lv_obj_set_style_image_recolor_opa(label, LV_OPA_COVER, 0);

    animated_planet = planet;
    planet_next_frame = 1U;
    planet_paused_for_idle = false;
    lv_obj_add_event_cb(screen, wolfie_status_screen_event_cb, LV_EVENT_DELETE, NULL);
    planet_motion_timer = lv_timer_create(wolfie_planet_timer_cb,
                                           WOLFIE_ORBIT_FRAME_MS, NULL);

    /* 3 digits + percent, in physical reading order. Use hidden leading zeroes. */
    for (uint8_t i = 0; i < 3U; ++i) {
        battery_glyph[i] = wolfie_make_glyph(screen, &wolfie_digit_0_img,
                                               WOLFIE_BATTERY_X + 7 * i,
                                               WOLFIE_BATTERY_Y);
    }
    battery_glyph[3] = wolfie_make_glyph(screen, &wolfie_percent_img,
                                          WOLFIE_BATTERY_X + 21, WOLFIE_BATTERY_Y);
    usb_glyph = wolfie_make_glyph(screen, &wolfie_usb_img,
                                   WOLFIE_USB_X, WOLFIE_USB_Y);
    previous_level = -1;
    previous_usb = -1;
    wolfie_refresh_battery();
    battery_refresh_timer = lv_timer_create(wolfie_battery_timer_cb, 15000, NULL);

    /* Same single combined layer+BT object as current firmware. */
    wolfie_layer_status = wolfie_make_glyph(screen,
                                           &wolfie_status_off_qwerty_img,
                                           WOLFIE_STATUS_X, WOLFIE_STATUS_Y);
    wolfie_last_layer = -1;
    wolfie_last_connected = -1;
    wolfie_current_connected = 0;
    wolfie_bt_ticks = 0U;
    wolfie_refresh_layer_status(true);
    wolfie_layer_timer = lv_timer_create(wolfie_layer_timer_cb,
                                        WOLFIE_LAYER_REFRESH_MS, NULL);

    return screen;
}
