#include "mbed.h"

#define CMD_MEASURE_HUMIDITY 0xF5       // Measure humidity register
#define CMD_MEASURE_TEMPERATURE 0xF3    // Measure temperature register
#define SI7021_ADDRESS 0x40 << 1        // Temperature and Humidity I2C address

class TemperatureSensor {
private:
    I2C &i2c; // Referenz auf die I2C-Instanz
    float temperature, humidity;

    float max_value;       // Der höchste Helligkeitswert
    float min_value;       // Der niedrigste Helligkeitswert
    float mean_value;      // Der Mittelwert der Helligkeit
    float max_value_humid;       // Der höchste Helligkeitswert
    float min_value_humid;       // Der niedrigste Helligkeitswert
    float mean_value_humid;      // Der Mittelwert der Helligkeit
    int measurement_count; // Zähler der Messungen (für Mittelwertberechnung)

public:
    TemperatureSensor(I2C &i2c_instance);  // Konstruktor mit I2C-Referenz
    float readHumidity();
    float readTemperature();

    float getTemp(){return temperature;}
    float getHumid(){return humidity;}

    void update_values(float new_val);
    void update_values_humid(float new_val);

    // Getter-Methoden für max, min und mean Werte
    float get_max_value() { return max_value; }
    float get_min_value() { return min_value; }
    float get_mean_value() { return mean_value; }
    float get_max_value_humid() { return max_value_humid; }
    float get_min_value_humid() { return min_value_humid; }
    float get_mean_value_humid() { return mean_value_humid; }
    void clear_values();

};
