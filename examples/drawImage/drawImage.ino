/*
 * drawImage.ino
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
}

void loop()
{
/**************************************************************** 
 * Function Name: void LEDMatrix::drawImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *image, led_color color)
 * Description: Draw image.
 * Parameters: x: X Position(Pay attention to the image size)
 *             y: Y Position(Pay attention to the image size)
 *             width: the width of the image, Max:64
 *             height: the height of the image, Max:64
 *             image: start address of the image data
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
    
    led.clear();                            // clear screen
    led.drawImage(16, 2, 32, 20, seeedLogo_32x20, GREEN);       // draw image. This example provides an example image data. You can use your own image data. Note: Max:64*64!
    delay(2000);
    
/**************************************************************** 
 * Function Name: void LEDMatrix::setBrightness(uint8_t level)
 * Description: Adjust brightness.
 * Parameters: level: min:0; max:100.
 * Return: none
****************************************************************/

    for (i = 0; i <= 95; i++) 
    {
        led.setBrightness(i);       // adjust brightness. i: min:0; max:100.
        delay(50);
    }
    led.setBrightness(30);
    
    while(1);
}
