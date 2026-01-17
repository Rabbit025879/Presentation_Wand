#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

#include "Button.h"

namespace ButtonTask {
  void button_task_start(
    QueueHandle_t haptics_q, 
    QueueHandle_t laser_q, 
    QueueHandle_t hid_q, 
    EventGroupHandle_t eg, 
    InputEvent* input_event,
    SystemMode* mode, 
    uint8_t pin = BUTTON_PIN
  );
} // namespace ButtonTask

#endif // BUTTON_TASK_H