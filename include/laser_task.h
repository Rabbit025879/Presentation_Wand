#ifndef LASER_TASK_H
#define LASER_TASK_H

#include "utils.h"
#include "Laser.h"
#include "Button.h"

namespace LaserTask {
  void laser_task_start(QueueHandle_t q, EventGroupHandle_t eg, uint8_t pin);
} // namespace LaserTask

#endif // LASER_TASK_H