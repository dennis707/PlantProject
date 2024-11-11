#include "accelerometer.h"

Accelerometer::Accelerometer(I2C &i2c_instance) : i2c(i2c_instance) {}

void Accelerometer::initialize() {
    uint8_t data[2] = {REG_CTRL_REG_1, 0x01};
    writeRegs(data, 2);
}

uint8_t Accelerometer::getWhoAmI() {
    uint8_t who_am_i = 0;
    readRegs(REG_WHO_AM_I, &who_am_i, 1);
    return who_am_i;
}

float Accelerometer::getAccX() {
    return static_cast<float>(getAccAxis(REG_OUT_X_MSB)) / 4096.0 * 9.81;
}

float Accelerometer::getAccY() {
    return static_cast<float>(getAccAxis(REG_OUT_Y_MSB)) / 4096.0 * 9.81;
}

float Accelerometer::getAccZ() {
    return static_cast<float>(getAccAxis(REG_OUT_Z_MSB)) / 4096.0 * 9.81;
}

void Accelerometer::readRegs(int addr, uint8_t *data, int len) {
    char t[1] = {static_cast<char>(addr)};
    i2c.write(MMA8451_I2C_ADDRESS, t, 1, true); // Zahl 1 gibt die Länge der zu schreibenden Daten an, also die Anzahl der Bytes
    //Das true-Argument steht für einen "repeated start" auf dem I2C-Bus. Es sorgt dafür, dass nach dem Senden dieses Bytes kein "STOP"-Signal gesendet wird.
    i2c.read(MMA8451_I2C_ADDRESS, reinterpret_cast<char *>(data), len);
}

void Accelerometer::writeRegs(uint8_t *data, int len) {
    i2c.write(MMA8451_I2C_ADDRESS, reinterpret_cast<char *>(data), len);
}

int16_t Accelerometer::getAccAxis(uint8_t addr) {
    int16_t acc;
    uint8_t res[2];
    readRegs(addr, res, 2);

    acc = (res[0] << 6) | (res[1] >> 2); // um ein 14-Bit-Ergebnis (acc) zu erhalten.
    if (acc > UINT14_MAX / 2) {
        acc -= UINT14_MAX;
    }
    return acc;
}
