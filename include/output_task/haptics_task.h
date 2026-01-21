#ifndef HAPTICS_TASK_H
#define HAPTICS_TASK_H

#include "Haptics.h"

class DeviceManager;

class HapticsTask {
public:
  HapticsTask();
  
  void start(
    QueueHandle_t q, 
    DeviceManager* device_manager, 
    SystemMode* mode
  );

private:
  TaskHandle_t haptics_task_handle;
  QueueHandle_t haptics_queue;
  DeviceManager* device_manager;
  SystemMode* current_system_mode;

  void haptics_task_impl();
  
  static void haptics_task_static(void *arg);
};

#endif // HAPTICS_TASK_H