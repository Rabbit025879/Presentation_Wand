#include "output_task/ota_task.h"
#include "DeviceManager.h"
#include "Utils.h"

static OTATask* ota_task_instance = nullptr;

OTATask::OTATask()
  : ota_task_handle(NULL),
    device_manager(nullptr),
    initialized(false) {
}

void OTATask::start(DeviceManager* device_manager) {
  this->device_manager = device_manager;

  ota_task_instance = this;

  xTaskCreate(
    ota_task_static,
    "ota_task",
    4096,
    NULL,
    OTA_TASK_PRIORITY,
    &ota_task_handle
  );
}

void OTATask::ota_task_static(void *arg) {
  if (ota_task_instance) {
    ota_task_instance->ota_task_impl();
  }
}

void OTATask::ota_task_impl() {
  OTA ota;
  
  for(;;) {
    if(device_manager->isOTAEnabled()) {
      if(!initialized) {
        ota.begin();
        initialized = true;
        DEBUG_PRINTLN("[OTA Task] Started");
      }
      ota.loop();
    } else {
      if(initialized) {
        ota.end();
        initialized = false;
        DEBUG_PRINTLN("[OTA Task] Stopped");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}