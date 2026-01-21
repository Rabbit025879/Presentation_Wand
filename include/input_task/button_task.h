#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

#include "MultiButton.h"

class DeviceManager;

class ButtonTask {
public:
  ButtonTask();
  
  void start(
    QueueHandle_t haptics_q, 
    QueueHandle_t laser_q, 
    QueueHandle_t hid_q, 
    DeviceManager* device_manager,
    InputEvent* input_event,
    SystemMode* mode
  );

private:
  TaskHandle_t button_task_handle;
  QueueHandle_t haptics_queue;
  QueueHandle_t laser_queue;
  QueueHandle_t hid_queue;
  DeviceManager* device_manager;
  InputEvent* current_input_event;
  SystemMode* current_system_mode;

  InputEvent _lastInputEvent;
  InputMode _lastInputMode;

  bool in_tuning = false;
  uint8_t tuning_target = 0; // 0=haptics, 1=laser, 2=led

  void button_task_impl();
  void send_queue(QueueHandle_t queue, EventBits_t bit);
  void motion_control_specific();
  void handle_command_mode();
  
  static void button_task_static(void *arg);
};

#endif // BUTTON_TASK_H