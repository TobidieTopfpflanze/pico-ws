#include <stdio.h>
#include "pico/stdlib.h"

// 0 = low
// 1 = high
#define WS_T0L 0.75
#define WS_T0H 0.50
#define WS_T1L 0.45
#define WS_T1H 0.80
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
    // gpio_put(PICO_WS_LED_PIN, false);
    sio_hw->gpio_clr = (1 << PICO_WS_LED_PIN);
    sleep_us(WS_RST);
}

void ws_write_high() {
    // gpio_put(PICO_WS_LED_PIN, true);
    sio_hw->gpio_set = (1 << PICO_WS_LED_PIN);
    sleep_us(WS_T1H);
    // gpio_put(PICO_WS_LED_PIN, false);
    sio_hw->gpio_clr = (1 << PICO_WS_LED_PIN);
    sleep_us(WS_T1L);
}

void ws_write_low() {
    // gpio_put(PICO_WS_LED_PIN, true);
    sio_hw->gpio_set = (1 << PICO_WS_LED_PIN);
    sleep_us(WS_T0H);
    // gpio_put(PICO_WS_LED_PIN, false);
    sio_hw->gpio_clr = (1 << PICO_WS_LED_PIN);
    sleep_us(WS_T0H);
}

void ws_write_color(short color) {
    unsigned int mask;
    for (mask = 0x80; mask != 0; mask >>= 1) {
        if (color & mask) {
            ws_write_high();
        } else {
            ws_write_low();
        }
    }
}

void ws_write_led(short r, short g, short b) 
{
    ws_write_color(g);
    ws_write_color(b);
    ws_write_color(r);
    ws_write_reset();
    return; 
}

int main()
{
    stdio_init_all();
    pico_led_init();

    ws_write_reset();

    int col = 0;
    while (true) {
        col += 1;
        if (col >= 255) {
            col = 0;
        }

        ws_write_led(0x00, col, 0x00);
        sleep_ms(50);
        printf("Color: %d\n", col);
    }

    while (true) {
        sio_hw->gpio_set = (1 << PICO_DEFAULT_LED_PIN);

        for (size_t i = 0; i < 16 * 16; i++)
        {
            ws_write_color(0x00);
            ws_write_color(i % 256);
            ws_write_color(0xff);
        }

        ws_write_reset();

        sio_hw->gpio_clr = (1 << PICO_DEFAULT_LED_PIN);
    }
}
