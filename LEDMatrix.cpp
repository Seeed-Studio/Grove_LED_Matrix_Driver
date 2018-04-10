/*
 * LEDMatrix.cpp
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

#include "LEDMatrix.h"
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <stdio.h>

// Note: You must define a SoftwareSerial class object that the name must be "matrix_serial", 
//       but you can change the pin number according to the actual situation.
//SoftwareSerial matrix_serial(2, 3);       // Don't forget define in the demo file 


//LEDMatrix::LEDMatrix(uint8_t receivePin, uint8_t transmitPin)
LEDMatrix::LEDMatrix()
{
    matrix_serial.begin(19200);
}

/**************************************************************** 
 * Function Name: clear
 * Description: Clear screen.
 * Parameters: none
 * Return: none
****************************************************************/ 
void LEDMatrix::clear()
{
    matrix_serial.write(START_FLAG);
    matrix_serial.write(uint8_t(CLEAR_DISPLAY));
    matrix_serial.write(STOP_FLAG);
}

/**************************************************************** 
 * Function Name: setBrightness
 * Description: Adjust brightness.
 * Parameters: level: min:0; max:100.
 * Return: none
****************************************************************/
void LEDMatrix::setBrightness(uint8_t level)
{
    if (level < 0)
    {
        level = 0;
    }
    if (level > 100)
    {
        level = 100;
    }
    matrix_serial.write(START_FLAG);
    matrix_serial.write(uint8_t(SET_BRIGHTNESS));
    matrix_serial.write(uint8_t(level));
    matrix_serial.write(STOP_FLAG);
}

/**************************************************************** 
 * Function Name: drawPoint
 * Description: Draw LED Point.
 * Parameters: x: X Position
 *             y: Y Position
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
void LEDMatrix::drawPoint(uint8_t x, uint8_t y, led_color color)
{
    matrix_serial.write(START_FLAG);
    matrix_serial.write(uint8_t(DRAW_POINT));
    matrix_serial.write(uint8_t(x));
    matrix_serial.write(uint8_t(y));
    matrix_serial.write(uint8_t(color));
    matrix_serial.write(STOP_FLAG);
}

/**************************************************************** 
 * Function Name: drawLine
 * Description: Draw Line.
 * Parameters: x1: starting point X Position
 *             y1: starting point Y Position
 *             x2: terminal point X Position
 *             y2: terminal point Y Position
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
void LEDMatrix::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, led_color color)
{

    matrix_serial.write(START_FLAG);
    matrix_serial.write(uint8_t(DRAW_LINE));
    matrix_serial.write(uint8_t(x1));
    matrix_serial.write(uint8_t(y1));
    matrix_serial.write(uint8_t(x2));
    matrix_serial.write(uint8_t(y2));
    matrix_serial.write(uint8_t(color));
    matrix_serial.write(STOP_FLAG);
}

/**************************************************************** 
 * Function Name: drawCircle
 * Description: Draw Circle.
 * Parameters: x0: X Position of the circle centre
 *             y0: Y Position of the circle centre
 *             radius: radius of the circle
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
void LEDMatrix::drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, led_color color)
{
    matrix_serial.write(START_FLAG);
    matrix_serial.write(uint8_t(DRAW_CIRCLE));
    matrix_serial.write(uint8_t(x0));
    matrix_serial.write(uint8_t(y0));
    matrix_serial.write(uint8_t(radius));
    matrix_serial.write(uint8_t(color));
    matrix_serial.write(STOP_FLAG);
}

/**************************************************************** 
 * Function Name: displayChar
 * Description: Display character.
 * Parameters: x: X Position
 *             y: Y Position
 *             c: ASCII code:'c' or 0-127(Dec)
 *             font: FONT_6X8 \ FONT_8X12 \ FONT_10X16 \ FONT_12X16
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
void LEDMatrix::displayChar(uint8_t x, uint8_t y, char c, font_size font, led_color color)
{
    if ((c < 0) || (c > 127))
    {
        Serial.println("Error! ASCII Value Range(Dec):0-127");
        return;
    }
    matrix_serial.write(START_FLAG);
    matrix_serial.write(uint8_t(DISPLAY_CHAR));
    matrix_serial.write(uint8_t(x));
    matrix_serial.write(uint8_t(y));
    matrix_serial.write(uint8_t(c));
    matrix_serial.write(uint8_t(font));
    matrix_serial.write(uint8_t(color));
    matrix_serial.write(STOP_FLAG);
}

/**************************************************************** 
 * Function Name: displayString
 * Description: Display character string.
 * Parameters: x: X Position
 *             y: Y Position
 *             charString: start address of character string
 *             length: the length of character string
 *             font: FONT_6X8 \ FONT_8X12 \ FONT_10X16 \ FONT_12X16
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
void LEDMatrix::displayString(uint8_t x, uint8_t y, char *charString, uint8_t length, font_size font, led_color color)
{
    uint8_t i;
    for (i = 0; i <= length; i++) 
    {
        displayChar(x, y, *charString, font, color);
        switch (font)
        {
            case FONT_6X8:
                x += 6;
                break;
            case FONT_8X12:
                x += 8;
                break;
            case FONT_10X16:
                x += 10;
                break;
            case FONT_12X16:
                x += 12;
                break;
            default:
                break;
        }
        charString++;
    }
}

/**************************************************************** 
 * Function Name: displayLongInteger
 * Description: Display long integer.
 * Parameters: x: X Position
 *             y: Y Position
 *             l: long integer, 32 bits(4 bytes), from -2,147,483,648 to 2,147,483,647.
 *             font: FONT_6X8 \ FONT_8X12 \ FONT_10X16 \ FONT_12X16
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
void LEDMatrix::displayLongInteger(uint8_t x, uint8_t y, long l, font_size font, led_color color)
{
    char str[12] = {0};
    uint8_t i;
 
    i = snprintf(str, 12, "%ld", l);
/*
    Serial.print("i: ");
    Serial.println(i);
    Serial.print("str: ");
    Serial.println(str);
*/
    for (i = 0; str[i] != '\0'; i++) 
    {
        displayChar(x, y, str[i], font, color);
        switch (font)
        {
            case FONT_6X8:
                x += 6;
                break;
            case FONT_8X12:
                x += 8;
                break;
            case FONT_10X16:
                x += 10;
                break;
            case FONT_12X16:
                x += 12;
                break;
            default:
                break;
        }
    }
}

/**************************************************************** 
 * Function Name: drawExpression
 * Description: Draw expression. A total of 7 kinds of expressions.
 * Parameters: x: X Position(Pay attention to the expression image size)
 *             y: Y Position(Pay attention to the expression image size)
 *             e: SMILE_32X20 \ SAD_32X32 \ CUTE_64X14 \ CRY_48X13 \ COMMON_32X10 \ CONTENTED_64X12 \ SLEEP_48X13 
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
void LEDMatrix::drawExpression(uint8_t x, uint8_t y, expression e, led_color color)
{
    matrix_serial.write(START_FLAG);
    matrix_serial.write(uint8_t(DRAW_EXPRESSION));
    matrix_serial.write(uint8_t(x));
    matrix_serial.write(uint8_t(y));
    matrix_serial.write(uint8_t(e));
    matrix_serial.write(uint8_t(color));
    matrix_serial.write(STOP_FLAG);
}

/**************************************************************** 
 * Function Name: drawImage
 * Description: Draw image.
 * Parameters: x: X Position(Pay attention to the image size)
 *             y: Y Position(Pay attention to the image size)
 *             width: the width of the image, Max:64
 *             height: the height of the image, Max:64
 *             image: start address of the image data
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
void LEDMatrix::drawImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *image, led_color color)
{
    uint16_t i;
    if ((width > 64) || (height > 64))
    {
        Serial.println("Error! Max Image Size:64*64");
        return;
    }
    matrix_serial.write(START_FLAG);
    matrix_serial.write(uint8_t(DRAW_IMAGE));
    matrix_serial.write(uint8_t(x));
    matrix_serial.write(uint8_t(y));
    matrix_serial.write(uint8_t(width));
    matrix_serial.write(uint8_t(height));
    matrix_serial.write(uint8_t(color));
    for (i = 0; i < width*height; i++)
    {
        matrix_serial.write(uint8_t(*image));
        image++;
    }
    matrix_serial.write(STOP_FLAG);
}
/*
void LEDMatrix::test()
{
    char recvChar = 0;
    while(matrix_serial.available())
    {
        recvChar = matrix_serial.read();
    }
    Serial.print("recv: ");
    Serial.println( recvChar );
}
*/

