#include "mbed.h"
#include "gps.h"
#include "color.h"
#include "accelerometer.h"
#include "temperatur.h"

// Definition des Buttons
InterruptIn user_button(PB_2);

// Gemeinsame I2C-Instanz für alle Sensoren
I2C i2c(PB_7, PB_6);  // Dieselben I2C-Pins für alle Sensoren


// GPS-Objekt erstellen (definiere die TX-, RX- und Enable-Pins entsprechend deiner Hardware)
GPS gps(PA_9, PA_10, PA_12); // Beispiel-Pins, passe diese ggf. an deine Hardware an

// Thread für GPS
Thread gps_thread;

// Funktion für das GPS-Thread (aufruft GPS-Lesefunktion in einer Endlosschleife)
void gpsThreadFunction() {
    //while (true) {
        gps.readAndProcessGPSData();
        ThisThread::sleep_for(2000ms);  // Wartezeit zwischen GPS-Lesezyklen
    //}
}


// Definiere die verschiedenen Modi
enum Mode {
    TestMode = 0,
    NormalMode,
    AdvancedMode
};

// Initialer Modus ist TestMode
Mode current_mode = TestMode;

// Funktion, um den Modus zu wechseln
void change_mode() {
    current_mode = static_cast<Mode>((current_mode + 1) % 3);
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

    // Starte das GPS-Thread
    gps_thread.start(gpsThreadFunction);

    while (true) {
        switch (current_mode) {
            case TestMode: {
                // Farbsensor auslesen und Werte anzeigen
                uint16_t clear, red, green, blue;
                colorSensor.readColorData(clear, red, green, blue);
                printf("Clear: %d, Red: %d, Green: %d, Blue: %d\n", clear, red, green, blue);

                // Beschleunigungswerte auslesen und anzeigen
                printf("X: %.2f m/s^2, Y: %.2f m/s^2, Z: %.2f m/s^2\n",
                       accel.getAccX(), accel.getAccY(), accel.getAccZ());

                // Temperatur und Luftfeuchtigkeit auslesen und anzeigen
                float temperature = tempSensor.readTemperature();
                float humidity = tempSensor.readHumidity();
                printf("Temperature: %.2f°C, Humidity: %.2f%%\n", temperature, humidity);
                //gps.readAndProcessGPSData();
                break;
            }
            case NormalMode:
                // Aktionen für Normal Mode
                printf("Normal Mode active.\n");
                break;
            case AdvancedMode:
                // Aktionen für Advanced Mode
                printf("Advanced Mode active.\n");
                break;
        }

        ThisThread::sleep_for(2000ms);  // Wartezeit zwischen den Messungen
    }
}
