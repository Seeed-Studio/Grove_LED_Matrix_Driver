/*
 * test_demo.ino
 * A quick start example for test all functions of the Grove-Led Matrix. 
 * Description: This demo let you can send instruction 0-8 to test all functions of the Grove-Led Matrix, via the serial port.
 *              And you also can modify the function arguments to test.
 *
 * Copyright (c) 2016 seeed technology inc.
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


#include <SoftwareSerial.h>
#include "LEDMatrix.h"

// Note: You must define a SoftwareSerial class object that the name must be "matrix_serial", 
//       but you can change the pin number according to the actual situation.
#define RECEIVE_PIN      2
#define TRANSMIT_PIN     3
SoftwareSerial matrix_serial(RECEIVE_PIN, TRANSMIT_PIN);

LEDMatrix led;

const uint8_t seeedLogo_32x20 [] = {
0x00, 0x08, 0x40, 0x00, 0x00, 0x18, 0x60, 0x00,
0x00, 0x38, 0x70, 0x00, 0x00, 0x30, 0x30, 0x00,
0x00, 0x70, 0x38, 0x00, 0x00, 0x70, 0x38, 0x00,
0x10, 0x70, 0x38, 0x20, 0x10, 0x70, 0x38, 0x20,
0x18, 0x70, 0x38, 0x60, 0x18, 0x70, 0x38, 0x60,
0x1C, 0x70, 0x38, 0x60, 0x0C, 0x70, 0x38, 0xE0,
0x0E, 0x30, 0x38, 0xC0, 0x0E, 0x30, 0x31, 0xC0,
0x07, 0x18, 0x33, 0x80, 0x03, 0x98, 0x67, 0x80,
0x01, 0xC8, 0x47, 0x00, 0x00, 0xE0, 0x1E, 0x00,
0x00, 0x30, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup()
{
    Serial.begin(9600); 
    delay(100);
}

void loop()
{
    int i, j;
    char recvChar = 0;
    char *cString = "WRB@*&9";
    while (Serial.available())
    {
        recvChar = Serial.read();
    }
    Serial.print("Send: ");
    Serial.println(recvChar);
    
    switch (recvChar)
    {
        case '0':
            led.clear();                    // clear screen
            Serial.println( "0" );
            break;
            
        case '1':
            for (i = 0; i <= 95; i++) 
            {
                led.setBrightness(i);       // adjust brightness. i: min:0; max:100.
                delay(50);
            }
            led.setBrightness(30);
            Serial.println( "1" );
            break;
            
        case '2':
            for (j = 0; j <= 31; j++) 
            {
                for (i = 0; i <= 63; i++)
                {
                    led.drawPoint(i, j, GREEN);     // draw LED Point
                    delay(10);
                }
            }
/*          
            led.drawPoint(63, 31, GREEN);
            led.drawPoint(62, 30, GREEN);
            led.drawPoint(61, 29, GREEN);
            led.drawPoint(60, 28, GREEN);
            led.drawPoint(59, 27, RED);
*/
            Serial.println( "2" );
            break;
            
        case '3':
            led.drawLine(5, 3, 62, 3, RED);         // draw Line
            led.drawLine(5, 5, 62, 5, RED);
            led.drawLine(61, 1, 61, 30, GREEN);
            led.drawLine(62, 1, 62, 30, GREEN);
            led.drawLine(1, 1, 30, 30, GREEN);
            led.drawLine(0, 26, 5, 31, RED);
            Serial.println( "3" );
            break;
            
        case '4':
            led.drawCircle(31, 16, 10, RED);            // draw circle
            led.drawCircle(31, 16, 15, GREEN);
            led.drawCircle(31, 16, 3, GREEN);
            Serial.println( "4" );
            break;
            
        case '5':
            for (i = 0; i <= 127; i++)
            {
                led.displayChar(5, 0, i, FONT_6X8, RED);        // display Char. ASCII Value Range(Dec):0-127
                delay(2000);
            }
            Serial.println( "5" );
//          led.displayChar(5, 0, 'W', FONT_6X8, RED);
//          led.displayChar(5, 10, 65, FONT_6X8, RED);
//          led.displayChar(5, 20, 38, FONT_6X8, GREEN);
            break;
            
        case '6':
            led.drawExpression(0, 0, SMILE_32X20, GREEN);       // draw Expression. A total of 7 kinds of expressions. Pay attention to the expression image size    
            delay(2000);
            led.clear();
            led.drawExpression(0, 0, SAD_32X32, GREEN);
            delay(2000);
            led.clear();
            led.drawExpression(0, 0, CUTE_64X14, GREEN);
            delay(2000);
            led.clear();
            led.drawExpression(0, 0, CRY_48X13, GREEN);
            delay(2000);
            led.clear();
            led.drawExpression(0, 0, COMMON_32X10, GREEN);
            delay(2000);
            led.clear();
            led.drawExpression(0, 0, CONTENTED_64X12, GREEN);
            delay(2000);
            led.clear();
            led.drawExpression(0, 0, SLEEP_48X13, GREEN);
            delay(2000);
            led.clear();
            Serial.println( "6" );
            break;
            
        case '7':
            led.drawImage(16, 2, 32, 20, seeedLogo_32x20, GREEN);       // draw image. This example provides an example image data. You can use your own image data. Note: Max:64*64!
            Serial.println( "7" );
            break;

        case '8':
            led.displayString(0, 0, cString, 7, FONT_6X8, GREEN);       // display String
            led.displayString(0, 10, "a!@#$%^&*", 9, FONT_10X16, GREEN);
            Serial.println( "8" );
            break;
            
        default:
            break;
    }
    
    delay(1000);
}
