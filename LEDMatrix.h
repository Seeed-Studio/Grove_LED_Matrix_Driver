/*
 * LEDMatrix.h
 * A library for Grove-Led Matrix
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wuruibin
 * Created Time: Jan 2016
 * Modified Time:
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __LED_MATRIX_H__
#define __LED_MATRIX_H__


#include <SoftwareSerial.h>

#define CLEAR_DISPLAY   0
#define SET_BRIGHTNESS  1
#define DRAW_POINT      2
#define DRAW_LINE       3
#define DRAW_CIRCLE     4
#define DISPLAY_CHAR    5
#define DRAW_EXPRESSION 6
#define DRAW_IMAGE      7
#define START_FLAG      125
#define STOP_FLAG       126

extern SoftwareSerial matrix_serial;

typedef enum 
{
    RED   = 1,
    GREEN = 2
} led_color;

typedef enum 
{
    FONT_6X8   = 2,
    FONT_8X12  = 3,
    FONT_10X16 = 4,
    FONT_12X16 = 5
} font_size;

typedef enum 
{
    SMILE_32X20     = 1,
    SAD_32X32       = 2,
    CUTE_64X14      = 3,
    CRY_48X13       = 4,
    COMMON_32X10    = 5,
    CONTENTED_64X12 = 6,
    SLEEP_48X13     = 7
} expression;


class LEDMatrix 
{
public:
    LEDMatrix();
    
    void clear();
    void setBrightness(uint8_t level);
    void drawPoint(uint8_t x, uint8_t y, led_color color);
    void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, led_color color);
    void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, led_color color);
    void displayChar(uint8_t x, uint8_t y, char c, font_size font, led_color color);
    void displayString(uint8_t x, uint8_t y, char *charString, uint8_t length, font_size font, led_color color);
    void displayLongInteger(uint8_t x, uint8_t y, long l, font_size font, led_color color);
    void drawExpression(uint8_t x, uint8_t y, expression e, led_color color);
    void drawImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *image, led_color color);
    
//  void test();
};


#endif
