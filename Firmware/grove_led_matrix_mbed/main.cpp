/**
    LED Matrix library for http://www.seeedstudio.com/depot/ultrathin-16x32-red-led-matrix-panel-p-1582.html
    The LED Matrix panel has 32x16 pixels. Several panel can be combined together as a large screen.

    Coordinate & Connection (mbed -> panel 0 -> panel 1 -> ...)
     (0, 0)                                     (0, 0)
       +--------+--------+--------+               +--------+--------+
       |   5    |    3   |    1   |               |    1   |    0   |
       |        |        |        |               |        |        |<----- mbed
       +--------+--------+--------+               +--------+--------+
       |   4    |    2   |    0   |                              (64, 16)
       |        |        |        |<----- mbed
       +--------+--------+--------+
                               (96, 32)

*/

#include <string.h>
#include "mbed.h"
#include "matrix.h"
#include "stm32f1xx.h"
#include "cmsis_nvic.h"
#include "font.h"
//#include "I2CSlave.h"

#define WIDTH   64
#define HEIGHT  32

#define CLEAR_DISPLAY   0
#define SET_BRIGHTNESS  1
#define DRAW_POINT      2
#define DRAW_LINE       3
#define DRAW_CIRCLE     4
#define DISPLAY_CHAR    5
#define DRAW_EXPRESSION 6
#define DRAW_IMAGE      7
#define START_FLAG      125
#define STOP_FLAG       126

Serial uart(PB_10, PB_11);
unsigned char message;
unsigned char messageBuf[16];
unsigned char imageBuf[64 * 64] = {0};
Matrix matrix;
uint32_t* red_matrix_buf;
uint32_t* green_matrix_buf;
TIM_HandleTypeDef timer_handle;

const unsigned char smile_32x20 [] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x00, 0x00, 0x3C, 0x78, 0x00,
    0x00, 0x3C, 0x78, 0x00, 0x00, 0x3C, 0x78, 0x00, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x06, 0x00, 0x00, 0x80, 0x03, 0x00, 0x01, 0x00, 0x01, 0x80, 0x03, 0x00,
    0x00, 0xC0, 0x0E, 0x00, 0x00, 0x60, 0x18, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x07, 0x80, 0x00
};

const unsigned char sad_32x32 [] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x18, 0x0C, 0x00,
    0x00, 0x60, 0x03, 0x00, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x20,
    0x04, 0x38, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x08, 0x08, 0x80, 0x00, 0x88,
    0x10, 0xD4, 0x2D, 0x04, 0x10, 0x7C, 0x3F, 0x04, 0x10, 0x7C, 0x1F, 0x04, 0x10, 0x38, 0x1E, 0x04,
    0x10, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x04,
    0x10, 0x03, 0xC0, 0x0C, 0x08, 0x0F, 0xF0, 0x08, 0x08, 0x18, 0x0C, 0x08, 0x04, 0x20, 0x06, 0x10,
    0x04, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x20, 0x01, 0x80, 0x00, 0xC0, 0x00, 0xC0, 0x01, 0x80,
    0x00, 0x30, 0x0E, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char contented_64x12 [] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x00, 0x01, 0xFF, 0xC0, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x00, 0x10, 0x08, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x80,
    0x02, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
};

const unsigned char cry_48x13 [] = {
    0x04, 0x00,
    0x00, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x00, 0x00, 0x04, 0x10, 0x7F, 0xC0, 0x01,
    0xFF, 0x04, 0x10, 0x7F, 0xC0, 0x01, 0xFF, 0x04, 0x08, 0x04, 0x00, 0x00, 0x10, 0x08, 0x08, 0x04,
    0x00, 0x00, 0x10, 0x08, 0x04, 0x04, 0x00, 0x00, 0x10, 0x10, 0x00, 0x04, 0x0F, 0xF8, 0x10, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x10, 0x00, 0x00, 0x04, 0x00, 0x00, 0x10, 0x00
};

const unsigned char common_32x10 [] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0xF0, 0x3F, 0x00, 0x01, 0xF8,
    0x3F, 0x00, 0x01, 0xF8, 0x3E, 0x00, 0x01, 0xF0, 0x1E, 0x3F, 0xF8, 0xF0, 0x00, 0x3F, 0xF8, 0x00,
    0x00, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char cute_64x14 [] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7B, 0xC0, 0x07, 0xBC, 0x00, 0x00,
    0x04, 0x00, 0x31, 0x80, 0x03, 0x18, 0x00, 0x40, 0x00, 0x00, 0x31, 0x80, 0x03, 0x18, 0x00, 0x20,
    0x08, 0x00, 0x31, 0x80, 0x03, 0x18, 0x00, 0x20, 0x08, 0x78, 0x31, 0x80, 0x03, 0x18, 0x3C, 0x20,
    0x08, 0xFC, 0x31, 0x80, 0x03, 0x18, 0x7E, 0x20, 0x08, 0xFC, 0x11, 0x00, 0x01, 0x10, 0x7E, 0x20,
    0x08, 0xF8, 0x0E, 0x00, 0x00, 0xE0, 0x7C, 0x20, 0x08, 0x78, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x20,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x40
};

const unsigned char sleep_48x13 [] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x08, 0x00, 0x00, 0x9F, 0x3E, 0x7C, 0x08, 0x00, 0x00, 0x83, 0x06, 0x0C,
    0x08, 0x00, 0x00, 0x86, 0x0C, 0x18, 0x08, 0xE0, 0x1C, 0x84, 0x08, 0x10, 0x08, 0x00, 0x00, 0x88,
    0x10, 0x20, 0x08, 0x00, 0x00, 0x90, 0x20, 0x40, 0x08, 0x00, 0x00, 0xBF, 0x7E, 0xFC, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xC1, 0x00, 0x00, 0x00
};

void tick_init(uint32_t period_us);

void tick_handler() {
    // Clear Update interrupt flag
    __HAL_TIM_CLEAR_FLAG(&timer_handle, TIM_FLAG_UPDATE);

    matrix.scan();
}

int main() {
    int i = 0;
    //    uint8_t count = 0;

    uart.baud(19200);

    matrix.begin(WIDTH, HEIGHT);

    matrix.setBrightness(30);

    red_matrix_buf = matrix.getRedBuffer();
    green_matrix_buf = matrix.getGreenBuffer();

    tick_init(1000);

    matrix.clear();

    while (1) {
        message = uart.getc();
        //      read(uint8_t *buffer, int length, const event_callback_t& callback, int event = SERIAL_EVENT_RX_COMPLETE, unsigned char char_match = SERIAL_RESERVED_CHAR_MATCH);
        if (message == START_FLAG) {
            messageBuf[0] = uart.getc();
            switch (messageBuf[0]) {
                case CLEAR_DISPLAY:
                    matrix.clear();
                    //                  uart.putc(0);
                    break;

                case SET_BRIGHTNESS:
                    messageBuf[1] = uart.getc();
                    matrix.setBrightness(messageBuf[1]);
                    //                  uart.putc(1);
                    //                  uart.putc(1);
                    break;

                case DRAW_POINT:
                    for (i = 1; i < 4; i++) {
                        messageBuf[i] = uart.getc();
                    }
                    if (messageBuf[3] == 1) {
                        matrix.drawPoint(messageBuf[1], messageBuf[2], 1, red_matrix_buf);
                    }
                    if (messageBuf[3] == 2) {
                        matrix.drawPoint(messageBuf[1], messageBuf[2], 1, green_matrix_buf);
                    }
                    matrix.requestUpdate();
                    //                  uart.putc(2);
                    break;

                case DRAW_LINE:
                    for (i = 1; i < 6; i++) {
                        messageBuf[i] = uart.getc();
                    }
                    if (messageBuf[5] == 1) {
                        matrix.line(messageBuf[1], messageBuf[2], messageBuf[3], messageBuf[4], 1, red_matrix_buf);
                    }
                    if (messageBuf[5] == 2) {
                        matrix.line(messageBuf[1], messageBuf[2], messageBuf[3], messageBuf[4], 1, green_matrix_buf);
                    }
                    matrix.requestUpdate();
                    //                  uart.putc(3);
                    break;

                case DRAW_CIRCLE:

                    for (i = 1; i < 5; i++) {
                        messageBuf[i] = uart.getc();
                    }
                    if (messageBuf[4] == 1) {
                        matrix.circle(messageBuf[1], messageBuf[2], messageBuf[3], 1, red_matrix_buf);
                    }
                    if (messageBuf[4] == 2) {
                        matrix.circle(messageBuf[1], messageBuf[2], messageBuf[3], 1, green_matrix_buf);
                    }
                    matrix.requestUpdate();
                    //                  uart.putc(4);
                    break;

                case DISPLAY_CHAR:
                    for (i = 1; i < 6; i++) {
                        messageBuf[i] = uart.getc();
                    }

                    if (messageBuf[5] == 1) {
                        switch (messageBuf[4]) {
                            /*                            case 1:
                                                            matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_4X6, red_matrix_buf);
                                                            break;
                            */
                            case 2:
                                matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_6X8, red_matrix_buf);
                                break;
                            case 3:
                                matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_8X12, red_matrix_buf);
                                break;
                            case 4:
                                matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_10X16, red_matrix_buf);
                                break;
                            case 5:
                                matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_12X16, red_matrix_buf);
                                break;
                            /*                            case 6:
                                                            matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_16X26, red_matrix_buf);
                                                            break;
                            */
                            default:
                                break;
                        }
                    }
                    if (messageBuf[5] == 2) {
                        switch (messageBuf[4]) {
                            /*                            case 1:
                                                            matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_4X6, green_matrix_buf);
                                                            break;
                            */
                            case 2:
                                matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_6X8, green_matrix_buf);
                                break;
                            case 3:
                                matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_8X12, green_matrix_buf);
                                break;
                            case 4:
                                matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_10X16, green_matrix_buf);
                                break;
                            case 5:
                                matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_12X16, green_matrix_buf);
                                break;
                            /*                            case 6:
                                                            matrix.putChar(messageBuf[1], messageBuf[2], messageBuf[3], FONT_16X26, green_matrix_buf);
                                                            break;
                            */
                            default:
                                break;
                        }
                    }
                    matrix.requestUpdate();
                    //                  uart.putc(5);
                    break;

                case DRAW_EXPRESSION:
                    for (i = 1; i < 5; i++) {
                        messageBuf[i] = uart.getc();
                    }

                    if (messageBuf[4] == 1) {
                        switch (messageBuf[3]) {
                            case 1:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 32, 20, smile_32x20, red_matrix_buf);
                                break;
                            case 2:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 32, 32, sad_32x32, red_matrix_buf);
                                break;
                            case 3:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 64, 14, cute_64x14, red_matrix_buf);
                                break;
                            case 4:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 48, 13, cry_48x13, red_matrix_buf);
                                break;
                            case 5:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 32, 10, common_32x10, red_matrix_buf);
                                break;
                            case 6:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 64, 12, contented_64x12, red_matrix_buf);
                                break;
                            case 7:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 48, 13, sleep_48x13, red_matrix_buf);
                                break;
                            default:
                                break;
                        }
                    }
                    if (messageBuf[4] == 2) {
                        switch (messageBuf[3]) {
                            case 1:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 32, 20, smile_32x20, green_matrix_buf);
                                break;
                            case 2:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 32, 32, sad_32x32, green_matrix_buf);
                                break;
                            case 3:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 64, 14, cute_64x14, green_matrix_buf);
                                break;
                            case 4:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 48, 13, cry_48x13, green_matrix_buf);
                                break;
                            case 5:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 32, 10, common_32x10, green_matrix_buf);
                                break;
                            case 6:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 64, 12, contented_64x12, green_matrix_buf);
                                break;
                            case 7:
                                matrix.drawImage(messageBuf[1], messageBuf[2], 48, 13, sleep_48x13, green_matrix_buf);
                                break;
                            default:
                                break;
                        }
                    }
                    matrix.requestUpdate();
                    //                  uart.putc(6);
                    break;

                case DRAW_IMAGE:
                    for (i = 1; i < 6; i++) {
                        messageBuf[i] = uart.getc();
                    }
                    memset(imageBuf, 0, 64 * 64);
                    for (i = 0; i < messageBuf[3]*messageBuf[4]; i++) {
                        imageBuf[i] = uart.getc();
                    }

                    if (messageBuf[5] == 1) {
                        matrix.drawImage(messageBuf[1], messageBuf[2], messageBuf[3], messageBuf[4], imageBuf, red_matrix_buf);
                    }
                    if (messageBuf[5] == 2) {
                        matrix.drawImage(messageBuf[1], messageBuf[2], messageBuf[3], messageBuf[4], imageBuf, green_matrix_buf);
                    }
                    matrix.requestUpdate();
                    break;

                default:
                    //                  uart.putc(9);
                    break;
            }

            message = uart.getc();
            if (message != STOP_FLAG) {
                while (uart.readable()) {
                    uart.getc();
                }
            }
        }

        matrix.requestUpdate();

        //        count = (count + 1) & 0x7;
        /*
                if(count % 2 == 1)
                {
                    matrix.drawPoint(0, 0, 1, red_matrix_buf);
                    matrix.requestUpdate();
                    matrix.drawPoint(1, 1, 1, red_matrix_buf);
                    matrix.requestUpdate();
                }
                else
                {
                    matrix.drawPoint(0, 0, 0, red_matrix_buf);
                    matrix.requestUpdate();
                    matrix.drawPoint(1, 1, 0, red_matrix_buf);
                    matrix.requestUpdate();
                }
        */
    }
}

void tick_init(uint32_t period_us) {
    // Enable timer clock
    __TIM2_CLK_ENABLE();

    // Reset timer
    __TIM2_FORCE_RESET();
    __TIM2_RELEASE_RESET();

    // Update the SystemCoreClock variable
    SystemCoreClockUpdate();

    // Configure time base
    timer_handle.Instance = TIM2;
    timer_handle.Init.Period        = period_us;
    timer_handle.Init.Prescaler     = (uint32_t)(SystemCoreClock / 1000000) - 1; // 1 us tick
    timer_handle.Init.ClockDivision = 0;
    timer_handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&timer_handle);

    __HAL_TIM_SetAutoreload(&timer_handle, period_us);


    // Configure interrupts
    // Update interrupt used for 32-bit counter
    NVIC_SetVector(TIM2_IRQn, (uint32_t)tick_handler);
    NVIC_EnableIRQ(TIM2_IRQn);

    // Enable interrupts
    __HAL_TIM_ENABLE_IT(&timer_handle, TIM_IT_UPDATE); // For 32-bit counter

    // Enable timer
    HAL_TIM_Base_Start(&timer_handle);
}
