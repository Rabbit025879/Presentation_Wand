#ifndef LASER_TASK_H
#define LASER_TASK_H

#include "Laser.h"

class LaserTask {
public:
  LaserTask();
  
  void start(
    QueueHandle_t q, 
    EventGroupHandle_t eg, 
    SystemMode* mode
  );

private:
  TaskHandle_t laser_task_handle;
  QueueHandle_t laser_queue;
  EventGroupHandle_t device_mode_event_group;
  SystemMode* current_system_mode;

  void laser_task_impl();
  
  static void laser_task_static(void *arg);
};

#endif // LASER_TASK_H