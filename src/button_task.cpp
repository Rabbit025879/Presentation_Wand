#include "button_task.h"

// Static instance pointer for FreeRTOS callback
static ButtonTask* button_task_instance = nullptr;

ButtonTask::ButtonTask()
  : button_task_handle(NULL),
    haptics_queue(nullptr),
    laser_queue(nullptr),
    hid_queue(nullptr),
    device_mode_event_group(nullptr),
    current_input_event(nullptr),
    current_system_mode(nullptr) {
}

void ButtonTask::start(
  QueueHandle_t haptics_q, 
  QueueHandle_t laser_q, 
  QueueHandle_t hid_q, 
  EventGroupHandle_t eg, 
  InputEvent* input_event, 
  SystemMode* mode, 
  uint8_t pin
) {
  haptics_queue = haptics_q;
  laser_queue = laser_q;
  hid_queue = hid_q;
  device_mode_event_group = eg;
  current_input_event = input_event;
  current_system_mode = mode;

  button_task_instance = this;

  xTaskCreate(
    button_task_static,
    "button_task",
    3072,
    (void*)(uintptr_t)pin,
    BUTTON_TASK_PRIORITY,
    &button_task_handle
  );
}

void ButtonTask::button_task_static(void *arg) {
  if (button_task_instance) {
    button_task_instance->button_task_impl((uint8_t)(uintptr_t)arg);
  }
}

void ButtonTask::button_task_impl(uint8_t pin) {
  Button button(pin);
  InputEvent lastInputEvent{ButtonState{false, ButtonEvent::None}, MotionEvent::None};

  for(;;) {
    current_input_event->buttonState = button.getState();
    switch (current_system_mode->inputMode) {
      case InputMode::SimpleInput:
        // Haptics and Laser on press/release
        if(current_input_event->buttonState.isPressed != lastInputEvent.buttonState.isPressed) {
          if(xEventGroupGetBits(device_mode_event_group) & USING_HAPTICS)
            xQueueSend(haptics_queue, current_input_event, portMAX_DELAY); // Instant haptics feedback
          if(xEventGroupGetBits(device_mode_event_group) & USING_LASER)
            xQueueSend(laser_queue, current_input_event, portMAX_DELAY);   // Laser on
        }

        switch (current_input_event->buttonState.event) {
          case ButtonEvent::SingleClick:
            /* code */
            break;
          case ButtonEvent::DoubleClick:
            /* code */
            break;
          case ButtonEvent::TripleClick:
            /* code */
            break;
          case ButtonEvent::SingleLongPress:
            /* code */
            break;
          case ButtonEvent::DoubleLongPress:
            /* code */
            break;
          case ButtonEvent::TripleLongPress:
            current_system_mode->inputMode = InputMode::Command;
            Serial.println("Switched to Command Mode");
            break;
          case ButtonEvent::Hold:
            break;
          case ButtonEvent::OtherPattern:
            break;
          default:
            break;
        }

        // HID on event
        if(current_input_event->buttonState.event != lastInputEvent.buttonState.event && xEventGroupGetBits(device_mode_event_group) & USING_HID)
          xQueueSend(hid_queue, current_input_event, portMAX_DELAY);

        break;
      case InputMode::Command:
        // Haptics feedback on event
        if(current_input_event->buttonState.event != lastInputEvent.buttonState.event && xEventGroupGetBits(device_mode_event_group) & USING_HAPTICS)
          xQueueSend(haptics_queue, current_input_event, portMAX_DELAY); // Command haptics feedback

        // System event group setup
        switch (current_input_event->buttonState.event) {
          case ButtonEvent::SingleClick:
            toggle_event_group_bit(USING_HAPTICS);
            Serial.println("Toggled Haptics Mode");
            break;
          case ButtonEvent::DoubleClick:
            toggle_event_group_bit(USING_LASER);
            Serial.println("Toggled Laser Mode");
            break;
          case ButtonEvent::TripleClick:
            toggle_event_group_bit(USING_HID);
            Serial.println("Toggled HID Mode");
            break;
          case ButtonEvent::SingleLongPress:
            toggle_event_group_bit(USING_MPU);
            Serial.println("Toggled MPU Mode");
            break;
          case ButtonEvent::DoubleLongPress:
            toggle_event_group_bit(USING_WEB_SERVER);
            Serial.println("Toggled Web Server Mode");
            break;
          case ButtonEvent::TripleLongPress:
            toggle_event_group_bit(USING_OTA_UPLOAD);
            Serial.println("Toggled OTA Upload Mode");
            break;
          case ButtonEvent::Hold:
            current_system_mode->inputMode = InputMode::SimpleInput;
            Serial.println("Switched to Simple Input Mode");
            break;  
          case ButtonEvent::OtherPattern:
            break;
          default:
            break;
        }

        break;
      case InputMode::MotionControl:
        // Hold to activate motion control
        if(current_input_event->buttonState.event == ButtonEvent::Hold) {
          xEventGroupSetBits(device_mode_event_group, USING_MPU);
        } else if(current_input_event->buttonState.event == ButtonEvent::None && lastInputEvent.buttonState.event == ButtonEvent::Hold) {
          xEventGroupClearBits(device_mode_event_group, USING_MPU);
        }
        break;
      default:
        break;
    }
    lastInputEvent = *current_input_event;
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void ButtonTask::toggle_event_group_bit(EventBits_t bit) {
  if(xEventGroupGetBits(device_mode_event_group) & bit) {
    xEventGroupClearBits(device_mode_event_group, bit);
  } else {
    xEventGroupSetBits(device_mode_event_group, bit);
  }
}