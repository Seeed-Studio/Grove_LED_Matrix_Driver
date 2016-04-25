/*
 * drawLine.ino
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
 * Function Name: void LEDMatrix::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, led_color color)
 * Description: Draw Line.
 * Parameters: x1: starting point X Position
 *             y1: starting point Y Position
 *             x2: terminal point X Position
 *             y2: terminal point Y Position
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
    
    led.clear();                            // clear screen
    led.drawLine(5, 3, 62, 3, RED);         // draw Line
    led.drawLine(5, 5, 62, 5, RED);
    led.drawLine(61, 1, 61, 30, GREEN);
    led.drawLine(62, 1, 62, 30, GREEN);
    led.drawLine(1, 1, 30, 30, GREEN);
    led.drawLine(0, 26, 5, 31, RED);
    
    while(1);
}
