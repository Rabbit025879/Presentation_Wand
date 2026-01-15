#ifndef UI_TASK_CPP
#define UI_TASK_CPP

#include "Haptics.h"
#include "Button.h"

namespace HapticsTask {
  void haptics_task_start(QueueHandle_t q, EventGroupHandle_t eg, uint8_t pin);
} // namespace HapticsTask

#endif // UI_TASK_CPP