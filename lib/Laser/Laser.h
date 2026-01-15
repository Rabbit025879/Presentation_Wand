#ifndef LASER_H
#define LASER_H

#include "OutputDevice.h"

class Laser : public OutputDevice {
public:
  Laser(uint8_t pin);
};

#endif // LASER_H