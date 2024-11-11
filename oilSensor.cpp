#include "oilSensor.h"
AnalogIn _sensorPin(A0);

// Liest die Bodenfeuchtigkeit und gibt sie in Prozent zurück
float SoilSensor::readMoisture() {
    sens_val = _sensorPin.read() * 100.0f;
    return _sensorPin.read() * 100.0f;  // Wandelt die analoge Eingabe in Prozent um
}
