#ifndef OTA_TASK_H
#define OTA_TASK_H

#include "OTA.h"

class DeviceManager;

class OTATask {
public:
  OTATask();
  
  void start(DeviceManager* device_manager);

private:
  TaskHandle_t ota_task_handle;
  DeviceManager* device_manager;
  bool initialized;

  void ota_task_impl();
  
  static void ota_task_static(void *arg);
};

#endif // OTA_TASK_H