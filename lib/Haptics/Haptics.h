#ifndef HAPTICS_H
#define HAPTICS_H

#include "OutputDevice.h"

class Haptics : public OutputDevice {
public:
  Haptics(uint8_t pin);
};

#endif // HAPTICS_H