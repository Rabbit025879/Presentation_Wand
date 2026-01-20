#ifndef HAPTICS_H
#define HAPTICS_H

#include "OutputDevice.h"

class Haptics : public OutputDevice {
public:
  Haptics(uint8_t pin = HAPTICS_PIN);
};

#endif // HAPTICS_H