#include <stdio.h>
#include "pico/stdlib.h"
#include "ws-led.hpp"
#include "assets/eyes.hpp"
#include "assets/plant.hpp"
#include "assets/kirby.hpp"
#include "assets/creeper.hpp"
#include "assets/pac_man.hpp"

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

    WS_DRIVER ws_driver(PICO_WS_LED_PIN, 16, 16);

    unsigned int c_offset = 0;
    unsigned int c_offset_step = 1;

    while (true) {
        c_offset += c_offset_step;
        if (c_offset >= 255)
        {
            c_offset_step = -1;
        }

        if (c_offset <= 0)
        {
            c_offset_step = 1;
        }

        // for (size_t x = 0; x < ws_driver.get_width(); x++)
        // {
        //     for (size_t y = 0; y < ws_driver.get_height(); y++)
        //     {
        //         ws_driver.set_pixel(x, y,x * ws_driver.get_width(), c_offset, y * ws_driver.get_height());
        //     }
        // }

        // ws_driver.write_buffer();

        int num_frames = sizeof(eyes) / sizeof(eyes[0]);

        for (int i = 0; i < 3; ++i) {
            for (int frame = 0; frame < num_frames; ++frame) {
                printf("Displaying frame %d\n", frame);
                for (int x = 0; x < 16; ++x) {
                    for (int y = 0; y < 16; ++y) {
                        const unsigned char* pixel = eyes[frame][x][y];
                        unsigned char r = pixel[0];
                        unsigned char g = pixel[1];
                        unsigned char b = pixel[2];
                        ws_driver.set_pixel(x, y,r,g,b);
                    }
                }

                ws_driver.write_buffer();
                // frame timing
                sleep_ms(150);
            }
        }
        
        num_frames = sizeof(plant) / sizeof(plant[0]);
        for (int i = 0; i < 3; ++i) {
            for (int frame = 0; frame < num_frames; ++frame) {
                printf("Displaying frame %d\n", frame);
                for (int x = 0; x < 16; ++x) {
                    for (int y = 0; y < 16; ++y) {
                        const unsigned char* pixel = plant[frame][x][y];
                        unsigned char r = pixel[0];
                        unsigned char g = pixel[1];
                        unsigned char b = pixel[2];
                        ws_driver.set_pixel(x, y,r,g,b);
                    }
                }

                ws_driver.write_buffer();
                // frame timing
                sleep_ms(300);
            }
        }
        
        num_frames = sizeof(kirby) / sizeof(kirby[0]);
        for (int i = 0; i < 20; ++i) {
            for (int frame = 0; frame < num_frames; ++frame) {
                printf("Displaying frame %d\n", frame);
                for (int x = 0; x < 16; ++x) {
                    for (int y = 0; y < 16; ++y) {
                        const unsigned char* pixel = kirby[frame][x][y];
                        unsigned char r = pixel[0];
                        unsigned char g = pixel[1];
                        unsigned char b = pixel[2];
                        ws_driver.set_pixel(x, y,r,g,b);
                    }
                }

                ws_driver.write_buffer();
                // frame timing
                sleep_ms(33);
            }
        }

        num_frames = sizeof(pac_man) / sizeof(pac_man[0]);
        for (int frame = 0; frame < num_frames; ++frame) {
            printf("Displaying frame %d\n", frame);
            for (int x = 0; x < 16; ++x) {
                for (int y = 0; y < 16; ++y) {
                    const unsigned char* pixel = pac_man[frame][x][y];
                    unsigned char r = pixel[0];
                    unsigned char g = pixel[1];
                    unsigned char b = pixel[2];
                    ws_driver.set_pixel(x, y,r,g,b);
                }
            }

            ws_driver.write_buffer();
            // frame timing
            sleep_ms(300);
        }

        num_frames = sizeof(creeper) / sizeof(creeper[0]);
        for (int frame = 0; frame < num_frames; ++frame) {
            printf("Displaying frame %d\n", frame);
            for (int x = 0; x < 16; ++x) {
                for (int y = 0; y < 16; ++y) {
                    const unsigned char* pixel = creeper[frame][x][y];
                    unsigned char r = pixel[0];
                    unsigned char g = pixel[1];
                    unsigned char b = pixel[2];
                    ws_driver.set_pixel(x, y,r,g,b);
                }
            }

            ws_driver.write_buffer();
            // frame timing
            sleep_ms(300);
        }

        // for (size_t x = 0; x < 16; x++)
        // {
        //     for (size_t y = 0; y < 16; y++)
        //     {
        //         ws_driver.set_pixel(x, y,255, 255, 255);
        //     }
        // }
    }
}