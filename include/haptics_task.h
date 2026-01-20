#ifndef HAPTICS_TASK_H
#define HAPTICS_TASK_H

#include "Haptics.h"

class HapticsTask {
public:
  HapticsTask();
  
  void start(
    QueueHandle_t q, 
    EventGroupHandle_t eg, 
    SystemMode* mode
  );

private:
  TaskHandle_t haptics_task_handle;
  QueueHandle_t haptics_queue;
  EventGroupHandle_t device_mode_event_group;
  SystemMode* current_system_mode;

  void haptics_task_impl();
  bool execute_haptics(ButtonEvent evt, Haptics& haptics);
  
  static void haptics_task_static(void *arg);
};

#endif // HAPTICS_TASK_H