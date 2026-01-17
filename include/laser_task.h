#ifndef LASER_TASK_H
#define LASER_TASK_H

#include "Laser.h"

namespace LaserTask {
  void laser_task_start(
    QueueHandle_t q, 
    EventGroupHandle_t eg, 
    SystemMode* mode, 
    uint8_t pin = LASER_PIN
  );
} // namespace LaserTask

#endif // LASER_TASK_H