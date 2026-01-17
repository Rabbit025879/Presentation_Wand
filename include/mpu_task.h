#ifndef MPU_TASK_H
#define MPU_TASK_H

#include "MPU.h"

namespace MPUTask {
  void mpu_task_start(
    QueueHandle_t haptics_q, 
    QueueHandle_t laser_q, 
    QueueHandle_t hid_q, 
    EventGroupHandle_t eg,
    InputEvent* input_event, 
    SystemMode* mode
  );
} // namespace MPUTask

#endif // MPU_TASK_H