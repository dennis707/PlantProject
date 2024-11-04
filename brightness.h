#include "mbed.h"

class Brightness
{
private:
    float brightness_val; // 0-1
public:
    Brightness();
    void measure_brightness();
    float get_brightness() {return brightness_val;}
    void set_red();
    void set_green();
    void set_yellow();
    void set_blue();
    void turn_off_led();
};


