#include <algorithm>
#include <stdio.h>
#include "pico/stdlib.h"
#include "ws-led.hpp"

/* -------------------------------- */
// led definition
/* -------------------------------- */

WS_LED::WS_LED(WS_COLOR_TYPE color_type, unsigned char r, unsigned char g, unsigned char b, unsigned char w)
{
    this->color_type = color_type;
    this->set_color(r, g, b, w);
}

void WS_LED::set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char w)
{
    this->red = r;
    this->green = g;
    this->blue = b;
    this->white = w;
}

unsigned int WS_LED::get_buffer()
{
    unsigned int color = 0;

    switch (this->color_type) {
        case RGB:
            color = (this->red << 16) | (this->green << 8) | this->blue;
            break;
        case BGR:
            color = (this->blue << 16) | (this->green << 8) | this->red;
            break;
        case GBR:
            color = (this->green << 16) | (this->blue << 8) | this->red;
            break;
        case GRB:
            color = (this->green << 16) | (this->red << 8) | this->blue;
            break;
        default:
            printf("WS_LED: Invalid color type\n");
            return 0;
    }

    return color;
}

/* -------------------------------- */
// Matrix definition
/* -------------------------------- */

WS_MATRIX::WS_MATRIX(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    this->leds = new WS_LED*[width];

    for (unsigned int x = 0; x < width; x++) {
    this->leds[x] = new WS_LED[height];
            for (unsigned int y = 0; y < height; y++) {
            this->leds[x][y] = WS_LED(GRB, 0, 0, 0);
        }
    }
}

unsigned int WS_MATRIX::get_size()
{
    return this->width * this->height;
}

unsigned int WS_MATRIX::get_width()
{
    return this->width;
}

unsigned int WS_MATRIX::get_height()
{
    return this->height;
}

/* -------------------------------- */
// Driver definition
/* -------------------------------- */

WS_DRIVER::WS_DRIVER(unsigned int pin)
{
    this->pin = pin;
    this->matrix = WS_MATRIX(16, 16);
    this->buffer = new unsigned char[this->matrix.get_size() * 3]; // 3 bytes per LED (RGB); if white, use 4 bytes

    gpio_init(this->pin);
    gpio_set_dir(this->pin, GPIO_OUT);
}

void WS_DRIVER::ws_write_reset()
{
    gpio_put(this->pin, false);
    sleep_us(WS_RST);
}

void WS_DRIVER::ws_write_high()
{
    gpio_put(this->pin, true);
    asm volatile(WS_T1H_ASM);
    gpio_put(this->pin, false);
    asm volatile(WS_T1L_ASM);
}

void WS_DRIVER::ws_write_low()
{
    gpio_put(this->pin, true);
    asm volatile(WS_T0H_ASM);
    gpio_put(this->pin, false);
    asm volatile(WS_T0L_ASM);
}

void WS_DRIVER::create_buffer()
{
    unsigned int index = 0;

    for (unsigned int x = 0; x < this->matrix.get_width(); x++) {
        for (unsigned int y = 0; y < this->matrix.get_height(); y++) {
            unsigned int ledbuf = this->matrix.leds[x][y].get_buffer();
            if(x % 2 == 1) {
                ledbuf = this->matrix.leds[x][this->matrix.get_height() - y - 1].get_buffer();
            }
            
            this->buffer[index] = ledbuf >> 16 & 0xFF;
            index += 1;
            this->buffer[index] = ledbuf >> 8 & 0xFF;
            index += 1;
            this->buffer[index] = ledbuf & 0xFF;
            index += 1;
        }
    }
}

void WS_DRIVER::write_buffer()
{
    create_buffer();
    for (size_t i = 0; i < this->matrix.get_size() * 3; i++)
    {
        ws_write_color(this->buffer[i]);
    }
    ws_write_reset();
}

void WS_DRIVER::set_pixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char w)
{
    matrix.leds[x][y].set_color(r, g, b, w);
}

void WS_DRIVER::ws_write_color(unsigned char color)
{
    unsigned int mask;
    for (mask = 0x80; mask != 0; mask >>= 1) {
        if (color & mask) {
            ws_write_high();
        } else {
            ws_write_low();
        }
    }
}