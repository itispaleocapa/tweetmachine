/*
 * Copyright 2013 C&D Elettronica. All rights reserved.
 *
 * This file is part of the FlashMat project.
 */

#ifndef FMATDEF_H_
#define FMATDEF_H_

#include <inttypes.h>

/*
 * Settings for FlashMat hardware/software
 */

namespace FlashMat {

#define MATRIX_ROWS              8   // #rows in the physical RGB matrix (should be a multiple of 8)
#define MATRIX_COLS             32   // #cols in the physical RGB matrix (should be a multiple of 8)
#define TEXT_BUFFER_SIZE       255   // the max number of characters that can be stored as text (text is first sent to cell, then shown with specified color/position/font/...)
#define TEXT_PACKET_MAX_SIZE    31   // if the text buffer size is greater than the I2C buffer size, the text must be split into multiple packets
#define ANIM_MAX_ARGS          128   // max bytes that can be passed as argument to a predefined animation
#define FM_I2C_BUFFER_SIZE     200   // the maximum size of a packet sent from master to cell (includes packet header and any kind of data the packet contains)
#define FM_I2C_FREQ         400000L  // the I2C clock frequency

#define COLOR_DEPTH  4   // #bits per color (1, 2, 4 or 8)

#define CELL_I2C_DEF_ADDR       64

// Broadcast address
#define BROADCAST    0

// Define the RGB order in the frame buffers
// This is independent of the order in which the colors must be sent to
// the MY9221 drivers (which depends on the hardware only)
#define R 0
#define G 1
#define B 2

// frame buffer constants
#define FRAME_SIZE_FOR(depth)          (MATRIX_ROWS * MATRIX_COLS * 3 * depth / 8)
#define FRAME_SIZE_1_COLOR_FOR(depth)  (MATRIX_ROWS * MATRIX_COLS     * depth / 8)
#define FRAME_SIZE          FRAME_SIZE_FOR(COLOR_DEPTH)          // size in bytes of the frame buffer
#define FRAME_SIZE_1_COLOR  FRAME_SIZE_1_COLOR_FOR(COLOR_DEPTH)  // as above, but for just 1 color
#define MATRIX_PIXELS       (MATRIX_ROWS * MATRIX_COLS)          // the number of pixels in the matrix
#define MATRIX_PIXEL_BYTES  (MATRIX_PIXELS / 8)                  // the number of bytes needed to store 1bit per pixel (pixels are "packed" 8-by-8) (in 1 color plane; this is used for masks, not for RGB frames at color depth = 1)
#define MATRIX_ROW_SIZE     (MATRIX_COLS * COLOR_DEPTH   / 8)    // the number of bytes necessary to store 1 row of 1 color
#define SIZE_8x8_FOR(depth) (8 * 8 * 3 * depth / 8)              // the size of an 8x8 subframe
#define SIZE_8x8            SIZE_8x8_FOR(COLOR_DEPTH)            // the size of an 8x8 subframe

// Constants to access properties on X and Y axis (font spacing, etc.)
#define X 0
#define Y 1

// Utilities

#define MAKE_RGB(r, g, b)   { (R == 0) * (uint8_t)(r) + (G == 0) * (uint8_t)(g) + (B == 0) * (uint8_t)(b), \
                              (R == 1) * (uint8_t)(r) + (G == 1) * (uint8_t)(g) + (B == 1) * (uint8_t)(b), \
                              (R == 2) * (uint8_t)(r) + (G == 2) * (uint8_t)(g) + (B == 2) * (uint8_t)(b) }

} // namespace

#endif /* FMATDEF_H_ */
