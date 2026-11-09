/* Cassini right nice!view: selected BLE host profile BT1..BT5.
 * Large 5x7 pixel block letters, 17x9 upright A1 glyphs, MSB-first.
 * Index is 1-based for the user; ZMK API indices remain 0..4.
 * Selection is distinct from the connected/disconnected indicator.
 */
#pragma once
#include <stdint.h>
#include <lvgl.h>
#define CASSINI_BT_PROFILE_X 128
#define CASSINI_BT_PROFILE_Y 5

static const uint8_t cassini_bt_profile_1_bits[27] = {
    0x00, 0x00, 0x00, 0xf3, 0xe2, 0x00, 0x88, 0x86, 0x00,
    0x88, 0x82, 0x00, 0xf0, 0x82, 0x00, 0x88, 0x82, 0x00,
    0x88, 0x82, 0x00, 0xf0, 0x87, 0x00, 0x00, 0x00, 0x00,
};
static const lv_image_dsc_t cassini_bt_profile_1_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 17,
    .header.h = 9,
    .header.stride = 3,
    .data_size = sizeof(cassini_bt_profile_1_bits),
    .data = cassini_bt_profile_1_bits,
};

static const uint8_t cassini_bt_profile_2_bits[27] = {
    0x00, 0x00, 0x00, 0xf3, 0xe7, 0x00, 0x88, 0x88, 0x80,
    0x88, 0x80, 0x80, 0xf0, 0x81, 0x00, 0x88, 0x82, 0x00,
    0x88, 0x84, 0x00, 0xf0, 0x8f, 0x80, 0x00, 0x00, 0x00,
};
static const lv_image_dsc_t cassini_bt_profile_2_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 17,
    .header.h = 9,
    .header.stride = 3,
    .data_size = sizeof(cassini_bt_profile_2_bits),
    .data = cassini_bt_profile_2_bits,
};

static const uint8_t cassini_bt_profile_3_bits[27] = {
    0x00, 0x00, 0x00, 0xf3, 0xef, 0x00, 0x88, 0x80, 0x80,
    0x88, 0x80, 0x80, 0xf0, 0x87, 0x00, 0x88, 0x80, 0x80,
    0x88, 0x80, 0x80, 0xf0, 0x8f, 0x00, 0x00, 0x00, 0x00,
};
static const lv_image_dsc_t cassini_bt_profile_3_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 17,
    .header.h = 9,
    .header.stride = 3,
    .data_size = sizeof(cassini_bt_profile_3_bits),
    .data = cassini_bt_profile_3_bits,
};

static const uint8_t cassini_bt_profile_4_bits[27] = {
    0x00, 0x00, 0x00, 0xf3, 0xe1, 0x00, 0x88, 0x83, 0x00,
    0x88, 0x85, 0x00, 0xf0, 0x89, 0x00, 0x88, 0x8f, 0x80,
    0x88, 0x81, 0x00, 0xf0, 0x81, 0x00, 0x00, 0x00, 0x00,
};
static const lv_image_dsc_t cassini_bt_profile_4_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 17,
    .header.h = 9,
    .header.stride = 3,
    .data_size = sizeof(cassini_bt_profile_4_bits),
    .data = cassini_bt_profile_4_bits,
};

static const uint8_t cassini_bt_profile_5_bits[27] = {
    0x00, 0x00, 0x00, 0xf3, 0xef, 0x80, 0x88, 0x88, 0x00,
    0x88, 0x88, 0x00, 0xf0, 0x8f, 0x00, 0x88, 0x80, 0x80,
    0x88, 0x80, 0x80, 0xf0, 0x8f, 0x00, 0x00, 0x00, 0x00,
};
static const lv_image_dsc_t cassini_bt_profile_5_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 17,
    .header.h = 9,
    .header.stride = 3,
    .data_size = sizeof(cassini_bt_profile_5_bits),
    .data = cassini_bt_profile_5_bits,
};

static const lv_image_dsc_t *const cassini_bt_profile_images[5] = {
    &cassini_bt_profile_1_img,
    &cassini_bt_profile_2_img,
    &cassini_bt_profile_3_img,
    &cassini_bt_profile_4_img,
    &cassini_bt_profile_5_img
};
