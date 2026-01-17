#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

#include "Button.h"

class ButtonTask {
public:
  ButtonTask();
  
  void start(
    QueueHandle_t haptics_q, 
    QueueHandle_t laser_q, 
    QueueHandle_t hid_q, 
    EventGroupHandle_t eg, 
    InputEvent* input_event,
    SystemMode* mode, 
    uint8_t pin = BUTTON_PIN
  );

private:
  TaskHandle_t button_task_handle;
  QueueHandle_t haptics_queue;
  QueueHandle_t laser_queue;
  QueueHandle_t hid_queue;
  EventGroupHandle_t device_mode_event_group;
  InputEvent* current_input_event;
  SystemMode* current_system_mode;

  void button_task_impl(uint8_t pin);
  void toggle_event_group_bit(EventBits_t bit);
  
  static void button_task_static(void *arg);
};

#endif // BUTTON_TASK_H