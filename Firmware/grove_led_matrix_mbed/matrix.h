/**
    LED Matrix library for http://www.seeedstudio.com/depot/ultrathin-16x32-red-led-matrix-panel-p-1582.html
    The LED Matrix panel has 32x16 pixels. Several panel can be combined together as a large screen.

    Coordinate & Connection (mbed -> panel 0 -> panel 1 -> ...)
     (0, 0)                                     (0, 0)
       +--------+--------+--------+               +--------+--------+
       |   5    |    3   |    1   |               |    1   |    0   |
       |        |        |        |               |        |        |<----- mbed
       +--------+--------+--------+               +--------+--------+
       |   4    |    2   |    0   |                              (64, 16)
       |        |        |        |<----- mbed
       +--------+--------+--------+
                               (96, 32)
    Copyright (c) 2013 Seeed Technology Inc.
    @auther     Yihui Xiong
    @date       Nov 7, 2013
    @license    Apache
*/


#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "mbed.h"
#include "font.h"

#define MAX_WIDTH       128
#define MAX_HEIGHT      32

class Matrix {
  public:
    Matrix();

    /**
        set the display's display buffer and number, the buffer's size must be not less than 512 * number / 8 bytes
        @param displaybuf    display buffer
        @param number        panels' number
    */
    void begin(uint16_t width, uint16_t height);

    void setBrightness(int level);

    int getBrightness();

    void update();

    void requestUpdate() {
        needUpdate = 1;
    }

    uint32_t* getRedBuffer() {
        return draw_buf_red;
    }

    uint32_t* getGreenBuffer() {
        return draw_buf_green;
    }

    /**
        draw a point
        @param x     x
        @param y     y
        @param pixel 0: led off, >0: led on
    */
    void drawPoint(uint16_t x, uint16_t y, uint8_t pixel, uint32_t* buf);

    void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t pixel, uint32_t* buf);

    void circle(uint16_t x, uint16_t y, uint16_t radius, uint8_t pixel, uint32_t* buf);

    /**
        draw a rect
        @param (x1, y1)   top-left position
        @param (x2, y2)   bottom-right position, not included in the rect
        @param pixel      0: rect off, >0: rect on
    */
    void drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t pixel, uint32_t* buf);

    void fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t pixel, uint32_t* buf);

    /**
        draw a image
        @param (xoffset, yoffset)   top-left offset of image
        @param (width, height)      image's width and height
        @param pixels     contents, 1 bit to 1 led
    */
    void drawImage(uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height, const uint8_t* image,
                   uint32_t* buf);

    void putChar(uint16_t x, uint16_t y, unsigned char c, const font_t& font, uint32_t* buf);

    void putString(uint16_t x, uint16_t y, const char* str, const font_t& font, uint32_t* buf);

    /**
        Set screen buffer to zero
    */
    void clear();

    /**
        turn off 1/16 leds and turn on another 1/16 leds
    */
    void scan();

    void reverse();

    uint8_t isReversed();

    void on();

    void off();

  private:
    PortOut port;
    DigitalOut r1, r2, g1, g2, stb, clk;
    PwmOut oe;
    float brightness;
    uint32_t scan_buf_red[(MAX_WIDTH * MAX_HEIGHT) / 32];
    uint32_t draw_buf_red[(MAX_WIDTH * MAX_HEIGHT) / 32];
    uint32_t scan_buf_green[(MAX_WIDTH * MAX_HEIGHT) / 32];
    uint32_t draw_buf_green[(MAX_WIDTH * MAX_HEIGHT) / 32];
    uint16_t width;
    uint16_t height;
    uint8_t  mask;
    uint8_t  needUpdate;
    uint8_t  state;
};

#endif
