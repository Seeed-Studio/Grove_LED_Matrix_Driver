/*
 * displayString.ino
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
 * Function Name: void LEDMatrix::displayString(uint8_t x, uint8_t y, char *charString, uint8_t length, font_size font, led_color color)
 * Description: Display character string.
 * Parameters: x: X Position
 *             y: Y Position
 *             charString: start address of character string
 *             length: the length of character string
 *             font: FONT_6X8 \ FONT_8X12 \ FONT_10X16 \ FONT_12X16
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
    
    char *cString = "Seeed&9";
    
    led.clear();                            // clear screen
    led.displayString(0, 0, cString, 7, FONT_6X8, GREEN);       // display String
    led.displayString(0, 10, "a!@#$%^&*", 9, FONT_10X16, GREEN);

    while(1);
}
