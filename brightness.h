#include "mbed.h"

class Brightness
{
private:
    float brightness_val; // 0-1
public:
    Brightness();
    float measure_brightness();
    //float get_brightness() {return brightness_val;}
};


