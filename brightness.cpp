#include "brightness.h"

AnalogIn brightness_sensor(A2);

// Constructor definition
Brightness::Brightness(RGB &rgbinstance) : rgb(rgbinstance), brightness_val(0),max_value(0), min_value(100), mean_value(0), measurement_count(0){
    // Initialize brightness value and mode (e.g., Normal mode by default)
}

float Brightness::measure_brightness() 
{
    //while (true) 
    //{
        bright =  brightness_sensor.read();
        brightness_val = bright * 100.0f;  // Read brightness (0.0 - 1.0)
            
    //}
    return brightness_val;
}

// Funktion zum Aktualisieren von max, min und mean
void Brightness::update_values(float new_brightness)
{
    // Maximalwert aktualisieren
    if (new_brightness > max_value) {
        max_value = new_brightness;
    }

    // Minimalwert aktualisieren
    if (new_brightness < min_value) {
        min_value = new_brightness;
    }

    // Mittelwert berechnen
    mean_value = ((mean_value * measurement_count) + new_brightness) / (measurement_count + 1);
    measurement_count++;  // Anzahl der Messungen erhöhen
}

void Brightness::clear_values()
{
    max_value = 0;     
    min_value = 100;       
    mean_value = 0;      
    measurement_count = 0; 
}

void Brightness::check_limit()
{
    if(bright > upper_limit || bright < lower_limit)
    {
        rgb.set_brightnessColor(); // yellow
    }
    //else 
    //{
    //    rgb.turn_off_led();
    //}
    
}