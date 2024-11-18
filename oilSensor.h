#include "mbed.h"
#include "rgb.h"

class SoilSensor {
private:
    RGB &rgb;
    float sens_val;
    float soil_help;
    float max_value;       
    float min_value;       
    float mean_value;      
    int measurement_count; 

    float upper_limit = 1;
    float lower_limit = 0;

public:
    SoilSensor(RGB &rgbinstance);
    // Methode zum Lesen der Bodenfeuchtigkeit in Prozent
    float readMoisture();
    float getSensorValue() {return sens_val;}

    void update_values(float new_moisture);

    // Getter-Methoden für max, min und mean Werte
    float get_max_value() { return max_value; }
    float get_min_value() { return min_value; }
    float get_mean_value() { return mean_value; }
    void clear_values();

    void check_limit();
};

