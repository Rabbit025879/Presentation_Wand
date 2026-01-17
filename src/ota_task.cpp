#include "ota_task.h"

static OTATask* ota_task_instance = nullptr;

OTATask::OTATask()
  : ota_task_handle(NULL),
    device_mode_event_group(nullptr),
    initialized(false) {
}

void OTATask::start(EventGroupHandle_t eg) {
  device_mode_event_group = eg;

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
    if(xEventGroupGetBits(device_mode_event_group) & USING_OTA_UPLOAD) {
      if(!initialized) {
        ota.begin();
        initialized = true;
      }
      ota.loop();
    } else {
      if(initialized) {
        ota.end();
        initialized = false;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}