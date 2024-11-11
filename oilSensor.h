#include "mbed.h"

class SoilSensor {
private:
    float sens_val;
public:
    // Methode zum Lesen der Bodenfeuchtigkeit in Prozent
    float readMoisture();
    float getSensorValue() {return sens_val;}
};

