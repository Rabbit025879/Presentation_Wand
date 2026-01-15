#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

#include "utils.h"
#include "Button.h"

namespace ButtonTask {
  void button_task_start(QueueHandle_t haptics_q, QueueHandle_t laser_q, QueueHandle_t hid_q, EventGroupHandle_t eg, uint8_t pin);
} // namespace ButtonTask

#endif // BUTTON_TASK_H