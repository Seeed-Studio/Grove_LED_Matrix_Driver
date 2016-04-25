/*
 * displayLongInteger.ino
 *
 * Copyright (c) 2016 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wuruibin
 * Created Time: Jan 2016
 * Modified Time:
 */

#include <SoftwareSerial.h>
#include "LEDMatrix.h"

// Note: You must define a SoftwareSerial class object that the name must be "matrix_serial", 
//       but you can change the pin number according to the actual situation.
#define RECEIVE_PIN      2
#define TRANSMIT_PIN     3
SoftwareSerial matrix_serial(RECEIVE_PIN, TRANSMIT_PIN);

LEDMatrix led;


void setup()
{
}

void loop()
{
/**************************************************************** 
 * Function Name: void LEDMatrix::displayLongInteger(uint8_t x, uint8_t y, long l, font_size font, led_color color)
 * Description: Display long integer.
 * Parameters: x: X Position
 *             y: Y Position
 *             l: long integer, 32 bits(4 bytes), from -2,147,483,648 to 2,147,483,647.
 *             font: FONT_6X8 \ FONT_8X12 \ FONT_10X16 \ FONT_12X16
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
    
    long l = -2147483647;
    int i = 999;
    
    led.clear();                            // clear screen
    led.displayLongInteger(0, 0, l, FONT_6X8, GREEN);
    led.displayLongInteger(0, 10, long(i), FONT_12X16, RED);
    
    while(1);
}
