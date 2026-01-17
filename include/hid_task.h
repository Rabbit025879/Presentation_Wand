#ifndef HID_TASK_H
#define HID_TASK_H

#include "BLE_HID.h"

class HIDTask {
public:
  HIDTask();
  
  void start(
    QueueHandle_t q, 
    EventGroupHandle_t eg, 
    SystemMode* mode
  );

private:
  TaskHandle_t hid_task_handle;
  QueueHandle_t hid_queue;
  EventGroupHandle_t device_mode_event_group;
  SystemMode* current_system_mode;

  void hid_task_impl();
  void hid_execute(ButtonEvent evt, BLEHID& blehid);
  
  static void hid_task_static(void *arg);
};

#endif // HID_TASK_H