#include "rgb.h"

// Pin definitions for RGB LED (common anode)
DigitalOut red_led(D15);
DigitalOut green_led(D13);
DigitalOut blue_led(D12);

void RGB::set_red()
{
    red_led = 0;   // Red (inverted for common anode)
    green_led = 1;
    blue_led = 1;
}

void RGB::set_green()
{
    red_led = 1;
    green_led = 0; // Green (inverted)
    blue_led = 1;
}

void RGB::set_blue()
{
    red_led = 1;   
    green_led = 1;
    blue_led = 0;
}

void RGB::turn_off_led()
{
    red_led = 1;   
    green_led = 1;
    blue_led = 1;
}

void RGB::set_tempColor()
{
    red_led = 0;   
    green_led = 1;
    blue_led = 0;
}

void RGB::set_accelColor()
{
    red_led = 0;   
    green_led = 0;
    blue_led = 0;
}

void RGB::set_brightnessColor()
{
    red_led = 0;   // Yellow (Red + Green, inverted)
    green_led = 0;
    blue_led = 1;
}

void RGB::set_soilColor()
{
    red_led = 1;   
    green_led = 0;
    blue_led = 0;
}