/* Compact 23x9 A1 Bluetooth connected/disconnected sprites.
 * Pre-rotated 180 degrees to match the working right-hand nice!view.
 * BT+ = active host profile connected, BT- = not connected.
 */
#pragma once
#include <stdint.h>
#include <lvgl.h>

static const uint8_t wolfie_bt_connected_bits[27] = {
    0x00, 0x00, 0x00, 0x00, 0x10, 0x78, 0x08, 0x10, 0x88, 0x08, 0x10, 0x88,
    0x3e, 0x10, 0x78, 0x08, 0x10, 0x88, 0x08, 0x10, 0x88, 0x00, 0x7c, 0x78,
    0x00, 0x00, 0x00,
};
static const lv_image_dsc_t wolfie_bt_connected_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 23,
    .header.h = 9,
    .header.stride = 3,
    .data_size = sizeof(wolfie_bt_connected_bits),
    .data = wolfie_bt_connected_bits,
};

static const uint8_t wolfie_bt_disconnected_bits[27] = {
    0x00, 0x00, 0x00, 0x00, 0x10, 0x78, 0x00, 0x10, 0x88, 0x00, 0x10, 0x88,
    0x3e, 0x10, 0x78, 0x00, 0x10, 0x88, 0x00, 0x10, 0x88, 0x00, 0x7c, 0x78,
    0x00, 0x00, 0x00,
};
static const lv_image_dsc_t wolfie_bt_disconnected_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 23,
    .header.h = 9,
    .header.stride = 3,
    .data_size = sizeof(wolfie_bt_disconnected_bits),
    .data = wolfie_bt_disconnected_bits,
};
