#include "mbed.h"

#define CMD_MEASURE_HUMIDITY 0xF5       // Measure humidity register
#define CMD_MEASURE_TEMPERATURE 0xF3    // Measure temperature register
#define SI7021_ADDRESS 0x40 << 1        // Temperature and Humidity I2C address

class TemperatureSensor {
private:
    I2C &i2c; // Referenz auf die I2C-Instanz

public:
    TemperatureSensor(I2C &i2c_instance);  // Konstruktor mit I2C-Referenz
    float readHumidity();
    float readTemperature();
};
