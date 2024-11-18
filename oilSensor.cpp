#include "oilSensor.h"
AnalogIn _sensorPin(A0);

SoilSensor::SoilSensor(RGB &rgbinstance) : rgb(rgbinstance)
{
    max_value = 0;     
    min_value = 100;       
    mean_value = 0;      
    measurement_count = 0; 
}

// Liest die Bodenfeuchtigkeit und gibt sie in Prozent zurück
float SoilSensor::readMoisture() {
    soil_help = _sensorPin.read();
    sens_val = soil_help * 100.0f;
    return sens_val;  // Wandelt die analoge Eingabe in Prozent um
}

// Funktion zum Aktualisieren von max, min und mean
void SoilSensor::update_values(float new_moisture)
{
    // Maximalwert aktualisieren
    if (new_moisture > max_value) {
        max_value = new_moisture;
    }

    // Minimalwert aktualisieren
    if (new_moisture < min_value) {
        min_value = new_moisture;
    }

    // Mittelwert berechnen
    mean_value = ((mean_value * measurement_count) + new_moisture) / (measurement_count + 1);
    measurement_count++;  // Anzahl der Messungen erhöhen
}

void SoilSensor::clear_values()
{
    max_value = 0;     
    min_value = 100;       
    mean_value = 0;      
    measurement_count = 0; 
}

void SoilSensor::check_limit()
{
    if(soil_help > upper_limit || soil_help < lower_limit)
    {
        rgb.set_soilColor(); // türkis
    }
    //else 
    //{
    //    rgb.turn_off_led();
    //}
    
}