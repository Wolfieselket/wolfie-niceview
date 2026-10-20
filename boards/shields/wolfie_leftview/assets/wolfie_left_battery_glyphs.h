/* Wolfie LEFT battery status: upright 5x7 pixel font in 7x9 A1 sprites.
 * The position is NOT mirrored; it matches the static scene's coordinates.
 * LVGL A1: one byte per row, seven active pixels, MSB first.
 */
#pragma once
#include <stdint.h>
#include <lvgl.h>
#define WOLFIE_GLYPH_W 7
#define WOLFIE_GLYPH_H 9

static const uint8_t wolfie_digit_0_bits[9] = { 0x00, 0x38, 0x44, 0x4c, 0x54, 0x64, 0x44, 0x38, 0x00 };
static const lv_image_dsc_t wolfie_digit_0_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_0_bits),
    .data = wolfie_digit_0_bits,
};

static const uint8_t wolfie_digit_1_bits[9] = { 0x00, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00 };
static const lv_image_dsc_t wolfie_digit_1_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_1_bits),
    .data = wolfie_digit_1_bits,
};

static const uint8_t wolfie_digit_2_bits[9] = { 0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c, 0x00 };
static const lv_image_dsc_t wolfie_digit_2_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_2_bits),
    .data = wolfie_digit_2_bits,
};

static const uint8_t wolfie_digit_3_bits[9] = { 0x00, 0x78, 0x04, 0x04, 0x38, 0x04, 0x04, 0x78, 0x00 };
static const lv_image_dsc_t wolfie_digit_3_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_3_bits),
    .data = wolfie_digit_3_bits,
};

static const uint8_t wolfie_digit_4_bits[9] = { 0x00, 0x08, 0x18, 0x28, 0x48, 0x7c, 0x08, 0x08, 0x00 };
static const lv_image_dsc_t wolfie_digit_4_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_4_bits),
    .data = wolfie_digit_4_bits,
};

static const uint8_t wolfie_digit_5_bits[9] = { 0x00, 0x7c, 0x40, 0x40, 0x78, 0x04, 0x04, 0x78, 0x00 };
static const lv_image_dsc_t wolfie_digit_5_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_5_bits),
    .data = wolfie_digit_5_bits,
};

static const uint8_t wolfie_digit_6_bits[9] = { 0x00, 0x18, 0x20, 0x40, 0x78, 0x44, 0x44, 0x38, 0x00 };
static const lv_image_dsc_t wolfie_digit_6_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_6_bits),
    .data = wolfie_digit_6_bits,
};

static const uint8_t wolfie_digit_7_bits[9] = { 0x00, 0x7c, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20, 0x00 };
static const lv_image_dsc_t wolfie_digit_7_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_7_bits),
    .data = wolfie_digit_7_bits,
};

static const uint8_t wolfie_digit_8_bits[9] = { 0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38, 0x00 };
static const lv_image_dsc_t wolfie_digit_8_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_8_bits),
    .data = wolfie_digit_8_bits,
};

static const uint8_t wolfie_digit_9_bits[9] = { 0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x08, 0x70, 0x00 };
static const lv_image_dsc_t wolfie_digit_9_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_digit_9_bits),
    .data = wolfie_digit_9_bits,
};

static const uint8_t wolfie_percent_bits[9] = { 0x00, 0x64, 0x68, 0x10, 0x10, 0x2c, 0x4c, 0x00, 0x00 };
static const lv_image_dsc_t wolfie_percent_img = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_A1,
    .header.w = 7,
    .header.h = 9,
    .header.stride = 1,
    .data_size = sizeof(wolfie_percent_bits),
    .data = wolfie_percent_bits,
};

static const lv_image_dsc_t *const wolfie_digits[10] = {
    &wolfie_digit_0_img,
    &wolfie_digit_1_img,
    &wolfie_digit_2_img,
    &wolfie_digit_3_img,
    &wolfie_digit_4_img,
    &wolfie_digit_5_img,
    &wolfie_digit_6_img,
    &wolfie_digit_7_img,
    &wolfie_digit_8_img,
    &wolfie_digit_9_img,
};
