#ifndef LASER_H
#define LASER_H

#include <Arduino.h>

class Laser {
public:
  Laser(uint8_t pin);
  void turnOn();
  void turnOff();

private:
  uint8_t _pin;
};

#endif // LASER_H