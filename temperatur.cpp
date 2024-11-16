#include "temperatur.h"

// Konstruktor: speichert die I2C-Referenz
TemperatureSensor::TemperatureSensor(I2C &i2c_instance, RGB &rgbinstance) : i2c(i2c_instance), rgb(rgbinstance) 
{
    max_value = -10;     
    min_value = 50;       
    mean_value = 0;      

    max_value_humid = 25;     
    min_value_humid = 75;       
    mean_value_humid = 0;   
    measurement_count = 0; 
}

// Methode zur Messung der Luftfeuchtigkeit
float TemperatureSensor::readHumidity() {                  
    char cmd[1] = { CMD_MEASURE_HUMIDITY };
    char data[2] = { 0 };
    
    i2c.write(SI7021_ADDRESS, cmd, 1); // Befehl zum Messen der Luftfeuchtigkeit senden 1--> Anzahl der Bytes
    ThisThread::sleep_for(20ms);       // Warten auf die Messung
    i2c.read(SI7021_ADDRESS, data, 2); // 2 Bytes für die Luftfeuchtigkeit lesen

    int humidity_raw = (data[0] << 8) | data[1];
    humidity = ((125.0 * humidity_raw) / 65536) - 6.0; // Umwandlung in %

    return humidity;
}

// Methode zur Messung der Temperatur
float TemperatureSensor::readTemperature() {
    char cmd[1] = { CMD_MEASURE_TEMPERATURE };
    char data[2] = { 0 };
    
    i2c.write(SI7021_ADDRESS, cmd, 1); // Befehl zum Messen der Temperatur senden
    ThisThread::sleep_for(20ms);       // Warten auf die Messung
    i2c.read(SI7021_ADDRESS, data, 2); // 2 Bytes für die Temperatur lesen

    int temperature_raw = (data[0] << 8) | data[1];
    temperature = ((175.72 * temperature_raw) / 65536) - 46.85; // Umwandlung in °C
    
    return temperature;
}


// Funktion zum Aktualisieren von max, min und mean
void TemperatureSensor::update_values(float new_val)
{
    // Maximalwert aktualisieren
    if (new_val > max_value) {
        max_value = new_val;
    }

    // Minimalwert aktualisieren
    if (new_val < min_value) {
        min_value = new_val;
    }

    // Mittelwert berechnen
    mean_value = ((mean_value * measurement_count) + new_val) / (measurement_count + 1);
    measurement_count++;  // Anzahl der Messungen erhöhen
}

void TemperatureSensor::update_values_humid(float new_val)
{
    // Maximalwert aktualisieren
    if (new_val > max_value_humid) {
        max_value_humid = new_val;
    }

    // Minimalwert aktualisieren
    if (new_val < min_value_humid) {
        min_value_humid = new_val;
    }

    // Mittelwert berechnen
    mean_value_humid = ((mean_value_humid * measurement_count) + new_val) / (measurement_count + 1);
    measurement_count++;  // Anzahl der Messungen erhöhen
}

void TemperatureSensor::clear_values()
{
    max_value = -10;     
    min_value = 50;       
    mean_value = 0;      

    max_value_humid = 25;     
    min_value_humid = 75;       
    mean_value_humid = 0;   
    measurement_count = 0; 
}

void TemperatureSensor::check_limit()
{
    if(temperature > upper_limit_temp || temperature < lower_limit_temp)
    {
        rgb.set_tempColor();
    }
    else 
    {
        rgb.turn_off_led();
    }

    if(humidity > upper_limit_humid || humidity < lower_limit_humid)
    {
        rgb.set_humidColor();
    }
    else 
    {
        rgb.turn_off_led();
    }
    
}