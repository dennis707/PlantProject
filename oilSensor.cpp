#include "oilSensor.h"
AnalogIn _sensorPin(PA_4);

// Liest die Bodenfeuchtigkeit und gibt sie in Prozent zurück
float SoilSensor::readMoisture() {
    return _sensorPin.read() * 100.0f;  // Wandelt die analoge Eingabe in Prozent um
}
