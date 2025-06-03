#include <stdio.h>
#include "pico/stdlib.h"
#include "ws-led.hpp"

#define LED_DELAY_US 1000000/2 // 1 second delay in microseconds
#define PICO_WS_LED_PIN 21

#ifndef PICO_DEFAULT_LED_PIN
#warning blink_simple example requires a board with a regular LED
#endif

// Initialize the GPIO for the LED
void pico_led_init(void) {
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

int main()
{
    stdio_init_all();
    pico_led_init();

    WS_DRIVER ws_driver(PICO_WS_LED_PIN);

    unsigned int c_offset = 0;
    unsigned int c_offset_step = 1;
    while (true) {
        printf("Starting WS LED test...\n");
        c_offset += c_offset_step;
        if (c_offset >= 255) {
            c_offset_step = -1;
        }
        
        if (c_offset <= 0) {
            c_offset_step = 1;
        }
        
        for (size_t x = 0; x < 16; x++)
        {
            for (size_t y = 0; y < 16; y++)
            {
                ws_driver.set_pixel(x, y,x * 16, c_offset, y * 16);
            }
        }

        ws_driver.write_buffer();
    }
}
