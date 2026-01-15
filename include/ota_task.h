#ifndef OTA_TASK_H
#define OTA_TASK_H

#include "utils.h"
#include "OTA.h"

namespace OTATask {
  void ota_task_start(EventGroupHandle_t eg);
} // namespace OTATask

#endif // OTA_TASK_H