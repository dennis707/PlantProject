#include "mbed.h"
#include "gps.h"
#include "color.h"
#include "accelerometer.h"
#include "temperatur.h"
#include "oilSensor.h"
#include "brightness.h"
#include "rgb.h"

// Definition des Buttons
InterruptIn user_button(PB_2);

// Definition der LED1 für das LoRaWAN-Board
DigitalOut led1(LED1);

// Gemeinsame I2C-Instanz für alle Sensoren
I2C i2c(PB_7, PB_6);  // Dieselben I2C-Pins für alle Sensoren

// GPS-Objekt erstellen (definiere die TX-, RX- und Enable-Pins entsprechend deiner Hardware)
GPS gps(PA_9, PA_10, PA_12); // Beispiel-Pins, passe diese ggf. an deine Hardware an

// Thread für GPS, Brightness und SoilSensor
Thread sensor_thread(osPriorityNormal, 1024); // stacksize 1 kilo 1024

// Neuer Thread für I2C-Sensoren
Thread i2c_thread(osPriorityNormal, 1024);

// Gemeinsame Variablen für die Sensorwerte
volatile uint16_t clear_val, red_val, green_val, blue_val;
volatile float acc_x, acc_y, acc_z;
volatile float temperature_val, humidity_val;
volatile float soil, brightness_val;

// Funktion für den kombinierten Sensor-Thread (GPS, Brightness und SoilSensor)
void sensorThreadFunction(GPS* gps, Brightness* brightness, SoilSensor* soilSensor) {
    while (true) {
        // GPS-Daten auslesen und verarbeiten
        gps->readAndProcessGPSData();
        
        // Helligkeit messen und ausgeben
        brightness_val = brightness->measure_brightness();

        // Bodenfeuchtigkeit messen und ausgeben
        float moisture = soilSensor->readMoisture();
        soil = moisture;

        ThisThread::sleep_for(2000ms);  // Wartezeit zwischen den Messungen
    }
}

void i2cThreadFunction(ColorSensor* colorSensor, Accelerometer* accel, TemperatureSensor* tempSensor) {
    while (true) {
        // Temporäre Variablen für Farbsensordaten ohne `volatile`
        uint16_t temp_clear, temp_red, temp_green, temp_blue;

        // Farbsensor auslesen und in temporäre Variablen speichern
        colorSensor->readColorData(temp_clear, temp_red, temp_green, temp_blue);

        // Bestimme die Farbe mit dem höchsten Wert und gebe die Farbe aus
        char maxColor;
        uint16_t maxValue;
        colorSensor->getMaxColor(temp_red, temp_green, temp_blue, maxColor, maxValue);

        // Werte in `volatile`-Variablen kopieren
        clear_val = temp_clear;
        red_val = temp_red;
        green_val = temp_green;
        blue_val = temp_blue;
        
        // Beschleunigungswerte auslesen
        acc_x = accel->getAccX();
        acc_y = accel->getAccY();
        acc_z = accel->getAccZ();
        
        // Temperatur und Luftfeuchtigkeit auslesen
        temperature_val = tempSensor->readTemperature();
        humidity_val = tempSensor->readHumidity();

        ThisThread::sleep_for(2000ms);  // Wartezeit zwischen den I2C-Messungen
    }
}

// Definiere die verschiedenen Modi
enum Mode {
    TestMode = 0,
    NormalMode
};

// Initialer Modus ist TestMode
Mode current_mode = TestMode;

// Funktion, um den Modus zu wechseln
void change_mode() {
    current_mode = (current_mode == TestMode) ? NormalMode : TestMode;
}

int main() {
    // Button-Interrupt konfigurieren (bei fallender Flanke)
    user_button.fall(&change_mode);

    // Farbsensor-Objekt erstellen und I2C-Instanz übergeben
    ColorSensor colorSensor(i2c);
    colorSensor.init();

    // Beschleunigungssensor-Objekt erstellen und I2C-Instanz übergeben
    Accelerometer accel(i2c);
    accel.initialize();
    printf("Accelerometer WhoAmI: %d\n", accel.getWhoAmI());

    // Temperatur- und Feuchtigkeitssensor-Objekt erstellen
    TemperatureSensor tempSensor(i2c);

    gps.initialize(); // GPS-Modul initialisieren

    // Brightness- und SoilSensor-Objekte erstellen
    Brightness brightness;
    SoilSensor soilSensor;

    // Starte den kombinierten Sensor-Thread mit GPS, Brightness und SoilSensor
    sensor_thread.start([&]() {
        sensorThreadFunction(&gps, &brightness, &soilSensor);
    });

    // Starte den I2C-Sensor-Thread für Farbsensor, Beschleunigungssensor und Temperatur-/Feuchtigkeitssensor
    i2c_thread.start([&]() {
        i2cThreadFunction(&colorSensor, &accel, &tempSensor);
    });

    // Hauptschleife mit Moduswechsel und nur Ausgaben
    while (true) {
        switch (current_mode) {
            case TestMode:
                led1 = 1;  // LED1 einschalten
                // Ausgeben der zuletzt ausgelesenen Werte der I2C-Sensoren
                printf("Brightness: %.2f\n", brightness_val);
                printf("Soil Moisture: %.2f%%\n", soil);
                printf("GPS: #Sats: %d Lat(UTC): %.6f %c Long(UTC): %.6f %c Altitude: %.1f %c GPT time: %s\n",
       gps.getNumSatellites(), gps.getLatitude(), gps.getParallel(), gps.getLongitude(), gps.getMeridian(), gps.getAltitude(), gps.getMeasurement(), gps.getGPSTime());
                printf("Clear: %d, Red: %d, Green: %d, Blue: %d\n", clear_val, red_val, green_val, blue_val);
                printf("X: %.2f m/s^2, Y: %.2f m/s^2, Z: %.2f m/s^2\n", acc_x, acc_y, acc_z);
                printf("Temperature: %.2f°C, Humidity: %.2f%%\n\n", temperature_val, humidity_val);
                break;
                
            case NormalMode:
                // Aktionen für Normal Mode
                led1 = 0;  // LED1 ausschalten
                printf("Normal Mode active.\n");
                break;
        }

        ThisThread::sleep_for(2000ms);  // Wartezeit zwischen den Ausgaben
    }
}
