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
    Copyright (c) 2013 Seeed Technology Inc.
    @auther     Yihui Xiong
    @date       Nov 8, 2013
    @license    Apache
*/

#include "matrix.h"
#include "mbed.h"


#define ASSERT(e)


Matrix::Matrix() :
    port(PortA, 0x0F), r1(PA_4), g1(PA_5), r2(PA_6), g2(PA_7), oe(PB_13), stb(PB_0), clk(PB_1) {
    oe.period_us(100); // not 100 us, it is 100 us / 72
    brightness = 0.5;
    mask = 0x00;
    state = 0;
    needUpdate = 0;

    width = MAX_WIDTH;
    height = MAX_HEIGHT;

    clear();
}

void Matrix::begin(uint16_t width, uint16_t height) {
    ASSERT(0 == (width % 32));
    ASSERT(0 == (height % 16));

    this->width = width;
    this->height = height;

    state = 1;
}

void Matrix::setBrightness(int level) {
    if (level < 0) {
        level = 0;
    } else if (level > 100) {
        level = 100;
    }

    brightness = level / 100.0;
}

int Matrix::getBrightness() {
    return brightness * 100;
}

void Matrix::update() {
    uint32_t* src_red = draw_buf_red;
    uint32_t* dst_red = scan_buf_red;
    uint32_t* src_green = draw_buf_green;
    uint32_t* dst_green = scan_buf_green;
    uint32_t n = width * height / 32;

    while (n--) {
        *dst_red++ = *src_red++;
        *dst_green++ = *src_green++;
    }

    needUpdate = 0;
}

void Matrix::drawPoint(uint16_t x, uint16_t y, uint8_t pixel, uint32_t* buf) {
    if (x >= width || y >= height) {
        return;
    }

    uint8_t* byte = (uint8_t*)buf + ((x + y * width) >> 3);
    uint8_t  bit = x % 8;

    if (pixel) {
        *byte |= 0x80 >> bit;
    } else {
        *byte &= ~(0x80 >> bit);
    }
}

void Matrix::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t pixel, uint32_t* buf) {
    int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

    dx = x2 - x1;
    dy = y2 - y1;
    if (dx > 0) {
        dxabs = dx;
        sdx = 1;
    } else {
        dxabs = -dx;
        sdx = -1;
    }
    if (dy > 0) {
        dyabs = dy;
        sdy = 1;
    } else {
        dyabs = -dy;
        sdy = -1;
    }

    x = dyabs >> 1;
    y = dxabs >> 1;
    px = x1;
    py = y1;

    drawPoint(px, py, pixel, buf);

    if (dxabs >= dyabs) { /* the line is more horizontal than vertical */
        for (i = 0; i < dxabs; i++) {
            y += dyabs;
            if (y >= dxabs) {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            /*
                if(i == 0)
                {
                }
                else
                {
                  px+=sdx;
                }*/
            drawPoint(px, py, pixel, buf);
        }
    } else { /* the line is more vertical than horizontal */
        for (i = 0; i < dyabs; i++) {
            x += dxabs;
            if (x >= dyabs) {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            /*
                if(i == 0)
                {
                }
                else
                {
                py+=sdy;
                }*/
            drawPoint(px, py, pixel, buf);
        }
    }
}

void Matrix::circle(uint16_t x0, uint16_t y0, uint16_t radius, uint8_t pixel, uint32_t* buf) {
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

    while (y <= x) {
        drawPoint(x + x0,  y + y0, pixel, buf);  // Octant 1
        drawPoint(y + x0,  x + y0, pixel, buf);  // Octant 2
        drawPoint(-x + x0,  y + y0, pixel, buf); // Octant 4
        drawPoint(-y + x0,  x + y0, pixel, buf); // Octant 3
        drawPoint(-x + x0, -y + y0, pixel, buf); // Octant 5
        drawPoint(-y + x0, -x + y0, pixel, buf); // Octant 6
        drawPoint(x + x0, -y + y0, pixel, buf);  // Octant 8
        drawPoint(y + x0, -x + y0, pixel, buf);  // Octant 7
        y++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
        } else {
            x--;
            decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
}

void Matrix::drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t pixel, uint32_t* buf) {
    for (uint16_t x = x1; x < x2; x++) {
        for (uint16_t y = y1; y < y2; y++) {
            drawPoint(x, y, pixel, buf);
        }
    }
}

void Matrix::fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t pixel, uint32_t* buf) {
    for (uint16_t i = 0; i < width; i++) {
        for (uint16_t j = 0; j < height; j++) {
            drawPoint(x + i, y + j, pixel, buf);
        }
    }
}

void Matrix::drawImage(uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height, const uint8_t* image,
                       uint32_t* buf) {
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            const uint8_t* byte = image + (x + y * width) / 8;
            uint8_t  bit = 7 - x % 8;
            uint8_t  pixel = (*byte >> bit) & 1;

            drawPoint(x + xoffset, y + yoffset, pixel, buf);
        }
    }
}

void Matrix::putChar(uint16_t x, uint16_t y, unsigned char c, const font_t& font, uint32_t* buf) {
    const uint8_t* data = font.data;
    uint16_t cw = font.char_width;
    uint16_t ch = font.char_height;
    uint16_t row_size = (cw + 7) / 8;
    uint16_t char_size = row_size * ch;
    uint16_t xo;

    fillRect(x, y, cw, ch, 0, buf);

    data += (c - font.start_char) * char_size;
    for (int row = 0; row < ch; row++) {
        for (int i = 0; i < row_size; i++) {
            uint8_t pixels = *data;
            xo = i * 8;
            while (pixels) {
                if (pixels & 1) {
                    drawPoint(x + xo, y + row, 1, buf);
                }

                xo++;

                pixels >>= 1;
            }
            data++;
        }
    }
}

void Matrix::putString(uint16_t x, uint16_t y, const char* str, const font_t& font, uint32_t* buf) {
    while (*str) {
        putChar(x, y, *str, font, buf);
        x += font.char_width;
        //y += font.char_height;

        str++;
    }
}


void Matrix::clear() {
    uint32_t* ptr_red = draw_buf_red;
    uint32_t* ptr_green = draw_buf_green;

    uint32_t n = width * height / 32;
    while (n--) {
        *ptr_red++ = 0;
        *ptr_green++ = 0;
    }

    needUpdate = 1;
}

void Matrix::reverse() {
    mask = ~mask;
}

uint8_t Matrix::isReversed() {
    return mask;
}

void Matrix::on() {
    state = 1;
}

void Matrix::off() {
    state = 0;
    oe = 1;
}

void Matrix::scan() {
    static uint8_t row = 0;

    if (!state) {
        return;
    }

    if (needUpdate) {
        update();
    }

    uint8_t* head_red = (uint8_t*)scan_buf_red + row * (width / 8);
    uint8_t* head_green = (uint8_t*)scan_buf_green + row * (width / 8);


    oe = 1.0;              // disable display

    for (uint8_t line = 0; line < (height / (16 * 2)); line++) {
        uint8_t* ptr_red = head_red;
        uint8_t* ptr_green = head_green;
        head_red += width * 2;              // width * 16 / 8

        for (uint8_t byte = 0; byte < (width / 8); byte++) {
            uint8_t pixels_red = *ptr_red;
            uint8_t pixels_green = *ptr_green;
            uint8_t pixels_red2 = *(ptr_red + width * 16 / 8);
            uint8_t pixels_green2 = *(ptr_green + width * 16 / 8);

            ptr_red++;
            ptr_green++;
            pixels_red = pixels_red ^ mask;   // reverse: mask = 0xff, normal: mask =0x00
            pixels_green = pixels_green ^ mask;
            for (uint8_t bit = 0; bit < 8; bit++) {
                clk = 0;
                r1 = pixels_red & (0x80 >> bit);
                g1 = pixels_green & (0x80 >> bit);
                r2 = pixels_red2 & (0x80 >> bit);
                g2 = pixels_green2 & (0x80 >> bit);
                clk = 1;
            }
        }

        clk = 0;
        head_red += width * 32 / 8;
        head_green += width * 32 / 8;
    }

    // select row
    port = row;

    // latch data
    stb = 0;
    stb = 1;

    oe = 1 - brightness;              // enable display
    //  oe = 0.9;

    row = (row + 1) & 0x0F;
}

