#include <stdio.h>
#include "pico/stdlib.h"

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

    int col = 0;
    while (true) {
        col += 1;
        if (col >= 255) {
            col = 0;
        }

        ws_write_led(col, 0x00, 0x00);
        ws_write_led(0x00, col, 0x00);
        ws_write_led(0x00, 0x00, col);
        ws_write_reset();

        printf("Color: %d\n", col);
        sleep_ms(50);
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
