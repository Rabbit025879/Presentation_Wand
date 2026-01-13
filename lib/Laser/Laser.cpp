#include "Laser.h"

Laser::Laser(uint8_t pin) : _pin(pin) {
  pinMode(_pin, OUTPUT);
}

void Laser::turnOn() {
  digitalWrite(_pin, HIGH);
}

void Laser::turnOff() {
  digitalWrite(_pin, LOW);
}