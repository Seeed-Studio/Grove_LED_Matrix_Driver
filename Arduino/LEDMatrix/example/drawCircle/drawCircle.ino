/*
 * drawCircle.ino
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
 * Function Name: void LEDMatrix::drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, led_color color)
 * Description: Draw Circle.
 * Parameters: x0: X Position of the circle centre
 *             y0: Y Position of the circle centre
 *             radius: radius of the circle
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/

    led.clear();                                // clear screen
    led.drawCircle(31, 16, 10, RED);            // draw circle
    led.drawCircle(31, 16, 15, GREEN);
    led.drawCircle(31, 16, 3, GREEN);
    
    while(1);
}
