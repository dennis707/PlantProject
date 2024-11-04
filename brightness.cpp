#include "brightness.h"

// Pin definitions for RGB LED (common anode)
DigitalOut red_led(D15);
DigitalOut green_led(D13);
DigitalOut blue_led(D12);
AnalogIn brightness_sensor(A0);

// Constructor definition
Brightness::Brightness() : brightness_val(0){
    // Initialize brightness value and mode (e.g., Normal mode by default)
}

void Brightness::measure_brightness() 
{
    while (true) 
    {
        
        brightness_val = brightness_sensor.read();  // Read brightness (0.0 - 1.0)
            
    }
}

void Brightness::set_red()
{
    red_led = 0;   // Red (inverted for common anode)
    green_led = 1;
    blue_led = 1;
}

void Brightness::set_green()
{
    red_led = 1;
    green_led = 0; // Green (inverted)
    blue_led = 1;
}

void Brightness::set_yellow()
{
    red_led = 0;   // Yellow (Red + Green, inverted)
    green_led = 0;
    blue_led = 1;
}

void Brightness::turn_off_led()
{
    red_led = 1;   
    green_led = 1;
    blue_led = 1;
}

void Brightness::set_blue()
{
    red_led = 1;   
    green_led = 1;
    blue_led = 0;
}
