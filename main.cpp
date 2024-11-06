#include "mbed.h"
#include "gps.h"
#include "color.h"
#include "accelerometer.h"

// Definition des Buttons
InterruptIn user_button(PB_2);

// Definiere die verschiedenen Modi
enum Mode {
    TestMode = 0,
    NormalMode,
    AdvancedMode
};

// Initialer Modus ist TestMode
Mode current_mode = TestMode;

// Kurze Funktion, die den Modus wechselt
void change_mode() {
    current_mode = static_cast<Mode>((current_mode + 1) % 3);
}

int main() {
    // Button-Interrupt konfigurieren (bei fallender Flanke)
    user_button.fall(&change_mode);

    // GPS-Objekt erstellen
    //GPS gps(PA_9, PA_10, PA_12);
    //gps.initialize();

    // Farbsensor-Objekt erstellen
    ColorSensor colorSensor(PB_7, PB_6); // I2C pins
    colorSensor.init(); // Initialize the color sensor

    // Accelerometer-Objekt erstellen
    Accelerometer accel(PB_9, PB_8);
    accel.initialize();
    printf("Accelerometer WhoAmI: %d\n", accel.getWhoAmI());

    // Separater Thread für die GPS-Datenverarbeitung
    //Thread gpsThread;
    //gpsThread.start(callback(&gps, &GPS::readAndProcessGPSData));

    while (true) {
        // Aktionen basierend auf dem Modus
        switch (current_mode) {
            case TestMode:
                colorSensor.run();
                //printf("%s\n\n", gps.get_data());
                printf("X: %.2f m/s^2 Y: %.2f m/s^2 Z: %.2f m/s^2\n", accel.getAccX(), accel.getAccY(), accel.getAccZ());
                break;
            case NormalMode:
                // Aktionen für Normal Mode
                break;
            case AdvancedMode:
                // Aktionen für Advanced Mode
                break;
        }

        // CPU entlasten
        ThisThread::sleep_for(100ms);
    }
}
