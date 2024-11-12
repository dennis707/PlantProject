#include "mbed.h"

class Brightness
{
private:
    float brightness_val; // 0-1

    float max_value;       // Der höchste Helligkeitswert
    float min_value;       // Der niedrigste Helligkeitswert
    float mean_value;      // Der Mittelwert der Helligkeit
    int measurement_count; // Zähler der Messungen (für Mittelwertberechnung)
public:
    Brightness();
    float measure_brightness();

    void update_values(float new_brightness);


    // Getter-Methoden für max, min und mean Werte
    float get_max_value() { return max_value; }
    float get_min_value() { return min_value; }
    float get_mean_value() { return mean_value; }
    void clear_values();
};


