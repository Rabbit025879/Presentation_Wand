#ifndef UI_TASK_CPP
#define UI_TASK_CPP

#include "Haptics.h"
#include "Button.h"

namespace HapticsTask {
  static void haptics_task(void *arg);
  void haptics_task_start(QueueHandle_t q, uint8_t pin);
  bool execute_haptics(ButtonEvent evt, Haptics& haptics);
} // namespace HapticsTask

namespace ButtonTask {
  static void button_task(void *arg);
  void button_task_start(QueueHandle_t q, uint8_t pin);
} // namespace ButtonTask

#endif // UI_TASK_CPP