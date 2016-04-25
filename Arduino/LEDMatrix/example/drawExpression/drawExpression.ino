/*
 * drawExpression.ino
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
 * Function Name: void LEDMatrix::drawExpression(uint8_t x, uint8_t y, expression e, led_color color)
 * Description: Draw expression. A total of 7 kinds of expressions.
 * Parameters: x: X Position(Pay attention to the expression image size)
 *             y: Y Position(Pay attention to the expression image size)
 *             e: SMILE_32X20 \ SAD_32X32 \ CUTE_64X14 \ CRY_48X13 \ COMMON_32X10 \ CONTENTED_64X12 \ SLEEP_48X13 
 *             color: led color, RED or GREEN.
 * Return: none
****************************************************************/
    
    led.clear();                            // clear screen
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

    while(1);
}
