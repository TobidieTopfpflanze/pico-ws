#ifndef WS_LED_H_
#define WS_LED_H_

enum WS_COLOR_TYPE
{
    RGB,
    BGR,
    GBR,
    GRB,
};

class WS_LED
{
private:
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char white; 
    enum WS_COLOR_TYPE color_type;

public:
    WS_LED(WS_COLOR_TYPE color_type = RGB, unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char w = 0);
    void set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char w = 0);
    unsigned int get_buffer();
};

class WS_MATRIX
{
private:
    unsigned int width;
    unsigned int height;

public:
    WS_MATRIX(unsigned int width = 0, unsigned int height = 0);
    WS_LED **leds;
    unsigned int get_size();
    unsigned int get_width();
    unsigned int get_height();

};

class WS_DRIVER
{
// timeout for 0.85ns
#define WS_T0L_ASM "nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n"
// timeout for 0.40ns
#define WS_T0H_ASM "nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n"
// timeout for 0.45ns
#define WS_T1L_ASM "nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n"
// timeout for 0.80ns
#define WS_T1H_ASM "nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n nop \n"

#define WS_RST 55
private:
    WS_MATRIX matrix;
    unsigned char *buffer;
    unsigned int pin;
    void ws_write_reset();
    void ws_write_high();
    void ws_write_low();
    void ws_write_color(unsigned char color);
    void create_buffer();

public:
    WS_DRIVER(unsigned int pin, unsigned int width = 0, unsigned int height = 0);
    void write_buffer();
    void set_pixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char w = 0);
    unsigned int get_width();
    unsigned int get_height();
};

#endif