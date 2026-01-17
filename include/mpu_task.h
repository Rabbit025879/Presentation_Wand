#ifndef MPU_TASK_H
#define MPU_TASK_H

#include "MPU.h"

class MPUTask {
public:
  MPUTask();
  
  void start(
    QueueHandle_t haptics_q, 
    QueueHandle_t laser_q, 
    QueueHandle_t hid_q, 
    EventGroupHandle_t eg,
    InputEvent* input_event, 
    SystemMode* mode
  );

private:
  TaskHandle_t mpu_task_handle;
  QueueHandle_t haptics_queue;
  QueueHandle_t laser_queue;
  QueueHandle_t hid_queue;
  EventGroupHandle_t device_mode_event_group;
  InputEvent* current_input_event;
  SystemMode* current_system_mode;

  void mpu_task_impl();
  
  static void mpu_task_static(void *arg);
};

#endif // MPU_TASK_H