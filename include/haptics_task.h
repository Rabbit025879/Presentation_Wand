#ifndef HAPTICS_TASK_H
#define HAPTICS_TASK_H

#include "Haptics.h"

namespace HapticsTask {
  void haptics_task_start(
    QueueHandle_t q, 
    EventGroupHandle_t eg, 
    SystemMode* mode, 
    uint8_t pin = HAPTICS_PIN
  );
} // namespace HapticsTask

#endif // HAPTICS_TASK_H