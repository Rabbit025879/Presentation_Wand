#ifndef OTA_TASK_H
#define OTA_TASK_H

#include "OTA.h"

class OTATask {
public:
  OTATask();
  
  void start(EventGroupHandle_t eg);

private:
  TaskHandle_t ota_task_handle;
  EventGroupHandle_t device_mode_event_group;
  bool initialized;

  void ota_task_impl();
  
  static void ota_task_static(void *arg);
};

#endif // OTA_TASK_H