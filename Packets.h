/*
 * Copyright 2013 C&D Elettronica. All rights reserved.
 *
 * This file is part of the FlashMat project.
 */

#ifndef PACKETS_H_
#define PACKETS_H_

#include <inttypes.h>

namespace FlashMat {

/*
 * Each packet is made of 1 byte (the Packet, identifying the packet type) and
 * by N bytes as parameters. Below are listed all the arguments and their use:
 *    name (intended type) description
 *    [# byte] name of the parameter to fit into this byte
 *
 * NOTE: every time an image or a color is sent in a packet, the RGB order is defined by
 * the R,G,B macros (see fmatdef.h). E.g. if R=0,G=1,B=2, the order is RGB; if it is
 * R=2,G=1,B=0, the order is BGR.
 *
 * Packets that can be sent from Commander to Cell:
 * + NONE : special value, not sent as a package but used as a "null" value
 *
 * + PING : void packet used only to test for an ACK from the slave;
 *                alternatively, the master can just send a packet with no data (address only)
 *
 * + SWAP : the cell must swap the back and front buffers as soon as possible.
 *     type (uint8_t) the OR of the SwapType bitmasks.
 *     [1] type
 *              NOTE: SWAP, when created by Creator for the commander, has also
 *                a timestamp as argument (uint16_t, as millis from the previous swap).
 *
 * + IMG_4bit_CHUNK : an 8x8 subframe of the full-color image.
 *     col (uint8_t) the x-index of the chunk, inside the cell's matrix (0-based, at each 8x8 chunk the index increases by 1, not by 8)
 *     row (uint8_t) the y-index of the chunk
 *     img (uint8_t[SIZE_8x8_FOR(4)]) the RGB image for this 8x8 chunk
 *     [1] col
 *     [2] row
 *     [3~SIZE_8x8_FOR(4)-1+3] img
 *
 * + FILL : uniform color for all pixels in the cell
 *     color (uint8_t[3])
 *     [1~3] color
 *
 * + TEXT_PARS  : text parameters:
 *     color (uint8_t[3]) the color for the text
 *     overlay (bool) if true the background is transparent (the current image is not overwritten)
 *     bgColor (uint8_t[3]) the uniform background (used only if overlay=false)
 *     fontId (Fonts::FontId) the font
 *     monospace (bool) monospace/variable-space
 *     charSpacing (uint8_t) the spacing between characters of the string
 *     lineSpacing (uint8_t) the spacing between lines
 *     [1~3] color
 *     [4]   overlay
 *     [5~7] bgColor
 *     [8]   fontId
 *     [9]   monospace
 *     [10]  charSpacing
 *     [11]  lineSpacing
 *                NOTE: all parameters are optional. If they are not sent, the current value is left unchanged.
 *
 * + TEXT : c-style string. Since the whole text might not fit into the I2C buffer,
 *              you can split it up in chunks of length TEXT_PACKET_MAX_SIZE.
 *              Each packet must contain the number in the group of related packets (those with
 *              chunks of the same string), and they *must* be send in order.
 *              Also, every packet (except maybe the last one) must have exactly TEXT_PACKET_MAX_SIZE characters.
 *              It is not necessary to send the terminating 0.
 *     number (uint8_t)
 *     text chunk (char[L])  (1<L<TEXT_PACKET_MAX_SIZE)
 *     [1] number
 *     [2~L+1] text chunk
 *
 * + CELL_POSITION : x-y position for this cell, i.e. the absolute coordinates of its top-left pixel.
 *     x (int16_t)
 *     y (int16_t)
 *     [1] MSB of x
 *     [2] LSB of x
 *     [3] MSB of y
 *     [4] LSB of y
 *
 * + POSITION : x-y position of the text, in absolute coordinates.
 *     x (int16_t)
 *     y (int16_t)
 *     [1] MSB of x
 *     [2] LSB of x
 *     [3] MSB of y
 *     [4] LSB of y
 *
 * + COPY_BUFFER : the front buffer is copied into the back buffer;
 *                 useful to avoid sending again the whole image if it is static, or if you want
 *                 to add something to it without having to redraw it from scratch
 *
 * + DRAW_TEXT : forces the cell to draw the text in the back buffer, using the already
 *                received parameters (text, text parameters, position):
 *
 * + PKT_DRAW_PIXEL
 *     x (int16_t)
 *     y (int16_t)
 *     color (uint8_t[3])
 *     [1] MSB of x
 *     [2] LSB of x
 *     [3] MSB of y
 *     [4] LSB of y
 *     [5~7] color
 *
 * + PKT_DRAW_LINE_H
 *     x1 (int16_t)
 *     x2 (int16_t)
 *     y (int16_t)
 *     color (uint8_t[3])
 *     [1] MSB of x1
 *     [2] LSB of x1
 *     [3] MSB of x2
 *     [4] LSB of x2
 *     [5] MSB of y
 *     [6] LSB of y
 *     [7~9] color
 *
 * + PKT_DRAW_LINE_V
 *     x (int16_t)
 *     y1 (int16_t)
 *     y2 (int16_t)
 *     color (uint8_t[3])
 *     [1] MSB of x
 *     [2] LSB of x
 *     [3] MSB of y1
 *     [4] LSB of y1
 *     [5] MSB of y2
 *     [6] LSB of y2
 *     [7~9] color
 *
 * + PKT_DRAW_RECT
 *     x1 (int16_t)
 *     y1 (int16_t)
 *     x2 (int16_t)
 *     y2 (int16_t)
 *     color (uint8_t[3])
 *     filled (bool)
 *     [1] MSB of x1
 *     [2] LSB of x1
 *     [3] MSB of y1
 *     [4] LSB of y1
 *     [5] MSB of x2
 *     [6] LSB of x2
 *     [7] MSB of y2
 *     [8] LSB of y2
 *     [9~11] color
 *     [12] filled
 *
 * + PKT_DRAW_GRADIENT : draw a color gradient from one color to another, in a given direction,
 *                        inside a given rectangular area
 *     color1 (uint8_t[3])
 *     color2 (uint8_t[3])
 *     type (Gradient)
 *     x1 (int16_t)
 *     y1 (int16_t)
 *     x2 (int16_t)
 *     y2 (int16_t)
 *     [1~3] color1
 *     [4~6] color2
 *     [7]  type
 *     [8]  MSB of x1
 *     [9]  LSB of x1
 *     [10] MSB of y1
 *     [11] LSB of y1
 *     [12] MSB of x2
 *     [13] LSB of x2
 *     [14] MSB of y2
 *     [15] LSB of y2
 *                NOTE: x1,y1,x2,y2 are optional; if not given, they default to the cell's boundaries
 *                      (which depend on the cell's size and the currently stored cellPosition)
 *
 * + PKT_DRAW_RAINBOW  : draw a horizontal color gradient from one color to another,
 *                       interpolating the hue of the HSV space, inside a given rectangular area.
 *                       All pixels will have the Value given, Saturation 1.0
 *                       and a Hue that is linearly interpolated between hue1 and hue2.
 *                       Hue is in [0.0,360.0), but hue1 and hue2 can have any value, to obtain
 *                       "clockwise" or "counter-clockwise" rotations of the hue and multiple
 *                       repetitions of the pattern.
 *                       Value is in [0.0,1.0].
 *     hue1 (float)
 *     hue2 (float)
 *     value (float)
 *     x1 (int16_t)
 *     y1 (int16_t)
 *     x2 (int16_t)
 *     y2 (int16_t)
 *     [1~4]  hue1
 *     [5~8]  hue2
 *     [9~12] value
 *     [13] MSB of x1
 *     [14] LSB of x1
 *     [15] MSB of y1
 *     [16] LSB of y1
 *     [17] MSB of x2
 *     [18] LSB of x2
 *     [19] MSB of y2
 *     [20] LSB of y2
 *                NOTE: x1,y1,x2,y2 are optional; if not given, they default to the cell's boundaries
 *                      (which depend on the cell's size and the currently stored cellPosition)
 *
 * + PKT_STORE_ADDRESS : give the cell a new address.
 *                       The address is changed immediately (no other commands are needed).
 *     i2c address (uint8_t; must be in [0,127], where 0 means "no address")
 *     <unused>
 *     <unused>
 *     [1] the value (~PKT_STORE_ADDRESS), given as control byte to avoid changing the address by mistake
 *     [2] i2c address
 *     [3] <unused>, set this to 0 as a default
 *     [4] <unused>, set this to 0 as a default
 */

    enum Packet {
        PKT_NONE            =  0,
        PKT_PING            =  1,
        PKT_SWAP            =  2,
        //PKT_IMG_8bit        = 10,
        //PKT_IMG_4bit        = 11,
        //PKT_IMG_2bit        = 12,
        //PKT_IMG_1bit        = 13,
        //PKT_IMG_8bit_CHUNK  = 14,
        PKT_IMG_4bit_CHUNK  = 15,
        //PKT_IMG_2bit_CHUNK  = 16,
        //PKT_IMG_1bit_CHUNK  = 17,
        //PKT_IMG_gray8       = 18,
        //PKT_IMG_gray4       = 19,
        //PKT_IMG_gray2       = 20,
        //PKT_IMG_gray1       = 21,
        //PKT_IMG_I2          = 22,
        //PKT_IMG_I4          = 23,
        //PKT_IMG_I16         = 24,
        //PKT_WHITE_BAL       = 30,
        PKT_FILL            = 40,
        PKT_COPY_BUFFER     = 41,
        PKT_CELL_POSITION   = 50,
        PKT_TEXT_POSITION   = 51,
        PKT_TEXT_PARS       = 60,
        PKT_TEXT            = 61,
        PKT_DRAW_TEXT       = 62,
        //PKT_DRAW_ANIMATION  = 70,
        PKT_DRAW_PIXEL      = 80,
        PKT_DRAW_LINE_H     = 81,
        PKT_DRAW_LINE_V     = 82,
        //PKT_DRAW_LINE       = 83,
        PKT_DRAW_RECT       = 84,
        //PKT_DRAW_ELLIPSE    = 85,
        PKT_DRAW_GRADIENT   = 90,
        PKT_DRAW_RAINBOW    = 91,
        PKT_STORE_ADDRESS   = 100,
        //PKT_REQ_STATUS      = 200,
        //PKT_WAIT            = 201,
        //PKT_BCAST_ADDRS     = 202
};

typedef uint8_t SwapType;
const SwapType SWAP_NOSYNC  = 0x00;
const SwapType SWAP_SYNC    = 0x01;
const SwapType SWAP_NOBLANK = 0x00;
const SwapType SWAP_BLANK   = 0x02;

}

#endif /* PACKETS_H_ */
