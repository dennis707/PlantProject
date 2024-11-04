#include "mbed.h"
#include "gps.h"
#include "color.h"

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
    GPS gps(PA_9, PA_10, PA_12);

    // GPS initialisieren
    gps.initialize();

    // Farbsensor-Objekt erstellen
    ColorSensor colorSensor(PB_7, PB_6); // I2C pins
    colorSensor.init(); // Initialize the color sensor

    // Separater Thread für die GPS-Datenverarbeitung
    Thread gpsThread;
    gpsThread.start(callback(&gps, &GPS::readAndProcessGPSData));

    // Thread für die Farbsensorverarbeitung starten
    Thread colorThread;
    colorThread.start(callback(&colorSensor, &ColorSensor::run));

    

    while (true) {
        // Hier könnten Aktionen basierend auf dem Modus ausgeführt werden
        switch (current_mode) {
            case TestMode:
                // Aktionen für Test Mode
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
