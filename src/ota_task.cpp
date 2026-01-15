#include "ota_task.h"

namespace OTATask {
  static TaskHandle_t ota_task_handle = NULL;
  static EventGroupHandle_t device_mode_event_group;

  static void ota_task(void *arg) {
    OTA ota;
    bool initialized = false;
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

  void ota_task_start(EventGroupHandle_t eg) {
    device_mode_event_group = eg;

    xTaskCreate(
      ota_task,
      "ota_task",
      4096,
      NULL,
      5,
      &ota_task_handle
    );
  }
} // namespace OTATask