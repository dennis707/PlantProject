#include "mbed.h"
#include "color.h"

// Constructor to initialize I2C and LED pin
ColorSensor::ColorSensor(PinName sda, PinName scl) : i2c(sda, scl), ledPin(PA_4), redCount(0), greenCount(0), blueCount(0) {
    // Empty constructor
}

// Function to write a value to a specified register
void ColorSensor::writeRegister(uint8_t reg, uint8_t value) {
    char data[2] = { (char)(TCS34725_COMMAND_BIT | reg), (char)value }; // Prepare the data to write
    i2c.write(TCS34725_ADDRESS, data, 2); // Write the data to the specified register
}

// Function to read a 16-bit value from a specified register
uint16_t ColorSensor::read16(uint8_t reg) {
    char cmd = TCS34725_COMMAND_BIT | reg; // Prepare the command to read from the register
    char data[2]; // Buffer to hold the read data
    i2c.write(TCS34725_ADDRESS, &cmd, 1); // Write the command to the sensor
    i2c.read(TCS34725_ADDRESS, data, 2); // Read 2 bytes of data
    return (data[1] << 8) | data[0]; // Combine the two bytes into a single 16-bit value
}

// Function to initialize the TCS34725 sensor
void ColorSensor::init() {
    writeRegister(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    ThisThread::sleep_for(3ms); // Wait for the sensor to power on
    writeRegister(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN); // Enable the ADC
    writeRegister(TCS34725_ATIME, 0xFF); // Max integration time (approximately 700 ms)
    writeRegister(TCS34725_CONTROL, 0x00); // Set gain to 1x
}

// Function to read the color data (clear, red, green, blue)
void ColorSensor::readColorData(uint16_t &clear, uint16_t &red, uint16_t &green, uint16_t &blue) {
    clear = read16(TCS34725_CDATAL); // Read the clear light value
    red = read16(TCS34725_RDATAL); // Read the red light value
    green = read16(TCS34725_GDATAL); // Read the green light value
    blue = read16(TCS34725_BDATAL); // Read the blue light value
}

// Determine dominant color and increment the corresponding counter
void ColorSensor::detectDominantColor(uint16_t red, uint16_t green, uint16_t blue) {
    if (red > green && red > blue) {
        redCount++;
    } else if (green > red && green > blue) {
        greenCount++;
    } else if (blue > red && blue > green) {
        blueCount++;
    }
}

// Function to run the color sensor in a thread
void ColorSensor::run() {
    uint16_t clear, red, green, blue; // Variables to hold color data

    //while (true) {
        // Read color data from the sensor
        readColorData(clear, red, green, blue);
        
        // Determine and log the dominant color
        detectDominantColor(red, green, blue);

        // Print the color values to the serial monitor
        printf("Clear: %d, Red: %d, Green: %d, Blue: %d\n", clear, red, green, blue);

        // Delay before reading again
        ThisThread::sleep_for(1000ms); // Wait for 1 second before the next reading
    //}
}
