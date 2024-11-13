#include "mbed.h"
#include "color.h"

// Konstruktor zur Initialisierung der I2C-Referenz und LED-Pin
ColorSensor::ColorSensor(I2C &i2c_instance, RGB &rgbInstance) : i2c(i2c_instance), rgb(rgbInstance), ledPin(PA_4), redCount(0), greenCount(0), blueCount(0) {
    // Optional: Weitere Initialisierungen
}

// Funktion zum Schreiben eines Wertes in ein bestimmtes Register
void ColorSensor::writeRegister(uint8_t reg, uint8_t value) {
    char data[2] = { (char)(TCS34725_COMMAND_BIT | reg), (char)value }; // Daten vorbereiten
    i2c.write(TCS34725_ADDRESS, data, 2); // Daten ins Register schreiben
}

// Funktion zum Lesen eines 16-Bit-Werts von einem bestimmten Register
uint16_t ColorSensor::read16(uint8_t reg) {
    char cmd = TCS34725_COMMAND_BIT | reg;
    char data[2];
    i2c.write(TCS34725_ADDRESS, &cmd, 1); // Leseanfrage senden
    i2c.read(TCS34725_ADDRESS, data, 2); // Daten lesen
    return (data[1] << 8) | data[0]; // Bytes zusammenführen
}

// Funktion zum Initialisieren des TCS34725-Sensors
void ColorSensor::init() {
    writeRegister(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    ThisThread::sleep_for(3ms);
    writeRegister(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN); // ADC aktivieren
    writeRegister(TCS34725_ATIME, 0xFF); // Maximale Integrationszeit
    writeRegister(TCS34725_CONTROL, 0x00); // Gain auf 1x setzen
}

// Funktion zum Lesen der Farbdaten
void ColorSensor::readColorData(uint16_t &clear, uint16_t &red, uint16_t &green, uint16_t &blue) {
    clear = read16(TCS34725_CDATAL);
    red = read16(TCS34725_RDATAL);
    green = read16(TCS34725_GDATAL);
    blue = read16(TCS34725_BDATAL);
}

// Dominante Farbe ermitteln und den entsprechenden Zähler inkrementieren
void ColorSensor::detectDominantColor(uint16_t red, uint16_t green, uint16_t blue) {
    if (red > green && red > blue) {
        redCount++;
    } else if (green > red && green > blue) {
        greenCount++;
    } else if (blue > red && blue > green) {
        blueCount++;
    }
}

// Funktion zur Ausführung der Sensorverarbeitung in einem Thread
void ColorSensor::run() {
    uint16_t clear, red, green, blue;

    readColorData(clear, red, green, blue);
    detectDominantColor(red, green, blue);

    printf("Clear: %d, Red: %d, Green: %d, Blue: %d\n", clear, red, green, blue);
    ThisThread::sleep_for(100ms);
}

// Funktion, die den höchsten Farbwert ermittelt und die Farbe ausgibt
void ColorSensor::getMaxColor(uint16_t red, uint16_t green, uint16_t blue, char &maxColor, uint16_t &maxValue) {
    // Initialisiere die Werte
    maxValue = red;
    maxColor = 'R'; // Start mit 'R' für Rot

    // Vergleiche mit Grün
    if (green > maxValue) {
        maxValue = green;
        maxColor = 'G'; // Grün ist der höchste Wert
    }

    // Vergleiche mit Blau
    if (blue > maxValue) {
        maxValue = blue;
        maxColor = 'B'; // Blau ist der höchste Wert
    }

    // Gebe den Namen der höchsten Farbe aus
    if (maxColor == 'R') {
        //printf("Rot hat den höchsten Wert: %d\n", maxValue);
        rgb.set_red();
    } else if (maxColor == 'G') {
        //printf("Grün hat den höchsten Wert: %d\n", maxValue);
        rgb.set_green();
    } else if (maxColor == 'B') {
        //printf("Blau hat den höchsten Wert: %d\n", maxValue);
        rgb.set_blue();
    }
}

int ColorSensor::get_max_counter()
{
    if (redCount > greenCount && redCount > blueCount) 
    {   
        // Sende die dominante Farbe an den Computer (oder verwende sie weiter)
        printf("Dominant Color is red: %d\n", redCount); // z.B. 1=Rot, 2=Grün, 3=Blau
        return redCount;
    } 
    else if (greenCount > redCount && greenCount > blueCount)
    {
        printf("Dominant Color is green: %d\n", greenCount); // z.B. 1=Rot, 2=Grün, 3=Blau
        return greenCount; 
    } 
    else if (blueCount > redCount && blueCount > greenCount)
    {
        printf("Dominant Color is blue: %d\n", blueCount); // z.B. 1=Rot, 2=Grün, 3=Blau
        return blueCount;
    } 
    return 1;
}