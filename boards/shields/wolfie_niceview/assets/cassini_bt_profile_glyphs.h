/* Cassini right nice!view: selected BLE host profile BT1..BT5.
 * Index is 1-based for the operator; ZMK API indices remain 0..4.
 * 17x9 upright A1 sprites, MSB-first, fixed placement in top-right.
 * These identify selected profiles, not whether that profile is connected.
 */
#pragma once
#include <stdint.h>
#include <lvgl.h>
#define CASSINI_BT_PROFILE_X 128
#define CASSINI_BT_PROFILE_Y 5

static const uint8_t cassini_bt_profile_1_bits[27] = {
    0x00, 0x00, 0x00, 0x31, 0xc4, 0x00, 0x28, 0x8c, 0x00, 0x30, 0x84, 0x00,
    0x28, 0x84, 0x00, 0x30, 0x8e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
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
    0x00, 0x00, 0x00, 0x31, 0xcc, 0x00, 0x28, 0x82, 0x00, 0x30, 0x84, 0x00,
    0x28, 0x88, 0x00, 0x30, 0x8e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
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
    0x00, 0x00, 0x00, 0x31, 0xcc, 0x00, 0x28, 0x82, 0x00, 0x30, 0x84, 0x00,
    0x28, 0x82, 0x00, 0x30, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
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
    0x00, 0x00, 0x00, 0x31, 0xca, 0x00, 0x28, 0x8a, 0x00, 0x30, 0x8e, 0x00,
    0x28, 0x82, 0x00, 0x30, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
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
    0x00, 0x00, 0x00, 0x31, 0xce, 0x00, 0x28, 0x88, 0x00, 0x30, 0x8c, 0x00,
    0x28, 0x82, 0x00, 0x30, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
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
