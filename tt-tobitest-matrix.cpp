#include <stdio.h>
#include "pico/stdlib.h"
#include "ws-led.hpp"

// 0 = low
// 1 = high

// timeout for 0.85ns
#define WS_T0L_ASM "nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n"
// timeout for 0.40ns
#define WS_T0H_ASM "nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n"
// timeout for 0.45ns
#define WS_T1L_ASM "nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n"
// timeout for 0.80ns
#define WS_T1H_ASM "nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n"

#define WS_RST 50
#define LED_DELAY_US 1000000/2 // 1 second delay in microseconds
#define PICO_WS_LED_PIN 21

#ifndef PICO_DEFAULT_LED_PIN
#warning blink_simple example requires a board with a regular LED
#endif

// Initialize the GPIO for the LED
void pico_led_init(void) {
    gpio_init(PICO_WS_LED_PIN);
    gpio_set_dir(PICO_WS_LED_PIN, GPIO_OUT);
#ifdef PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

// Turn the LED on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

void ws_write_reset() {
    gpio_put(PICO_WS_LED_PIN, false);
    sleep_us(WS_RST);
}

void ws_write_high() {
    gpio_put(PICO_WS_LED_PIN, true);
    asm volatile(WS_T1H_ASM);
    gpio_put(PICO_WS_LED_PIN, false);
    asm volatile(WS_T1L_ASM);
}

void ws_write_low() {
    gpio_put(PICO_WS_LED_PIN, true);
    asm volatile(WS_T0H_ASM);
    gpio_put(PICO_WS_LED_PIN, false);
    asm volatile(WS_T0L_ASM);
}

void ws_write_color(unsigned char color) {
    unsigned int mask;
    for (mask = 0x80; mask != 0; mask >>= 1) {
        if (color & mask) {
            ws_write_high();
        } else {
            ws_write_low();
        }
    }
}

void ws_write_led(int color_r, int color_g, int color_b) {
    ws_write_color(color_g);
    ws_write_color(color_b);
    ws_write_color(color_r);
    return; 
}

int main()
{
    stdio_init_all();
    pico_led_init();

    ws_write_reset();
    printf("done...\n");

    unsigned int width = 16;
    unsigned int height = 16;
    WS_LED **leds = new WS_LED*[width];

    for (unsigned int x = 0; x < width; x++) {
        leds[x] = new WS_LED[height];
        for (unsigned int y = 0; y < height; y++) {
            leds[x][y] = WS_LED(GRB, x, 0, y);
        }
    }

    unsigned int c_offset = 0;
    unsigned int c_offset_step = 1;

    unsigned char *buffer = new unsigned char[width * height * 3]; // 3 bytes per LED (RGB); if white, use 4 bytes
    printf("start\n");
    while (true) {
        c_offset += c_offset_step;
        if (c_offset >= 255) {
            c_offset_step = -1;
        }
        
        if (c_offset <= 0) {
            c_offset_step = 1;
        }

        for (unsigned int x = 0; x < width; x++) {
            for (unsigned int y = 0; y < height; y++) {
                leds[x][y].set_color(x * 16, c_offset, y * 16);
            }
        }

        unsigned int index = 0;

        for (unsigned int x = 0; x < width; x++) {
            for (unsigned int y = 0; y < height; y++) {
                unsigned int ledbuf = leds[x][y].get_buffer();
                if(x % 2 == 1) {
                    ledbuf = leds[x][height - y - 1].get_buffer();
                }
                buffer[index] = ledbuf >> 16 & 0xFF;
                index += 1;
                buffer[index] = ledbuf >> 8 & 0xFF;
                index += 1;
                buffer[index] = ledbuf & 0xFF;
                index += 1;
            }
        }

        for (size_t i = 0; i < width * height * 3; i++)
        {
            ws_write_color(buffer[i]);
        }

        ws_write_reset();
    }
}
