/*
 * drawPoint.ino
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
 * Function Name: LEDMatrix::drawPoint(uint8_t x, uint8_t y, led_color color)
 * Description: Draw LED Point.
 * Parameters: x: X Position
 *             y: Y Position
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/

    int i, j;
    
    led.clear();                            // clear screen
    for (j = 0; j <= 31; j++) 
    {
        for (i = 0; i <= 63; i++)
        {
            led.drawPoint(i, j, GREEN);     // draw LED Point
            delay(10);
        }
    }
    
    delay(1000);
    
    led.clear();                            // clear screen
    for (j = 0; j <= 31; j++) 
    {
        for (i = 0; i <= 63; i++)
        {
            led.drawPoint(i, j, RED);     // draw LED Point
            delay(10);
        }
    }
    
    while(1);
}
