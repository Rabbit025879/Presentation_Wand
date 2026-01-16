#ifndef HID_TASK_H
#define HID_TASK_H

#include "utils.h"
#include "BLE_HID.h"
#include "Button.h"

namespace HIDTask {
  static void hid_task(void *arg);
  void hid_task_start(QueueHandle_t q, EventGroupHandle_t eg, SystemMode* mode);
} // namespace HIDTask

#endif // HID_TASK_H