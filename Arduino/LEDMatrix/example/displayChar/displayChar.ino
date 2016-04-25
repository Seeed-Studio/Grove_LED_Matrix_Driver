/*
 * displayChar.ino
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
 * Function Name: void LEDMatrix::displayChar(uint8_t x, uint8_t y, char c, font_size font, led_color color)
 * Description: Display character.
 * Parameters: x: X Position
 *             y: Y Position
 *             c: ASCII code:'c' or 0-127(Dec)
 *             font: FONT_6X8 \ FONT_8X12 \ FONT_10X16 \ FONT_12X16
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/

    int i;
    
    led.clear();                            // clear screen
    led.displayChar(5, 0, 'W', FONT_6X8, RED);
    led.displayChar(5, 10, '8', FONT_6X8, RED);
    led.displayChar(5, 20, '&', FONT_8X12, GREEN);
    
    for (i = 0; i <= 127; i++)
    {
        led.displayChar(20, 0, i, FONT_8X12, GREEN);        // display Char. ASCII Value Range(Dec):0-127
        delay(2000);
    }

    while(1);
}
