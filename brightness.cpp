#include "brightness.h"

AnalogIn brightness_sensor(A2);

// Constructor definition
Brightness::Brightness() : brightness_val(0){
    // Initialize brightness value and mode (e.g., Normal mode by default)
}

void Brightness::measure_brightness() 
{
    //while (true) 
    //{
        
        brightness_val = brightness_sensor.read();  // Read brightness (0.0 - 1.0)
            
    //}
}


