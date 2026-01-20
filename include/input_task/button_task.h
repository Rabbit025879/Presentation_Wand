#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

#include "MultiButton.h"

class SettingsManager;

class ButtonTask {
public:
  ButtonTask();
  
  void start(
    QueueHandle_t haptics_q, 
    QueueHandle_t laser_q, 
    QueueHandle_t hid_q, 
    EventGroupHandle_t eg, 
    InputEvent* input_event,
    SystemMode* mode
  );

private:
  TaskHandle_t button_task_handle;
  QueueHandle_t haptics_queue;
  QueueHandle_t laser_queue;
  QueueHandle_t hid_queue;
  EventGroupHandle_t device_mode_event_group;
  InputEvent* current_input_event;
  SystemMode* current_system_mode;

  InputEvent _lastInputEvent;
  InputMode _lastInputMode;

  void button_task_impl();
  void toggle_event_group_bit(EventBits_t bit);
  void send_queue(QueueHandle_t queue, EventBits_t bit);
  void motion_control_specific();
  void handle_command_mode(DeviceSettings& settings, SettingsManager& sm, bool& in_settings, bool& in_tuning, uint8_t& tuning_target);
  
  static void button_task_static(void *arg);
};

#endif // BUTTON_TASK_H