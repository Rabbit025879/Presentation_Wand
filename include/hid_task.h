#ifndef HID_TASK_H
#define HID_TASK_H

#include "BLE_HID.h"

namespace HIDTask {
  void hid_task_start(
    QueueHandle_t q, 
    EventGroupHandle_t eg, 
    SystemMode* mode
  );
} // namespace HIDTask

#endif // HID_TASK_H