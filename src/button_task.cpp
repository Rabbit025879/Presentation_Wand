#include "button_task.h"

namespace ButtonTask {
static TaskHandle_t button_task_handle = NULL;
static QueueHandle_t haptics_queue;
static QueueHandle_t laser_queue;
static QueueHandle_t hid_queue;
static EventGroupHandle_t device_mode_event_group;
static SystemMode* current_system_mode;

void toggle_event_group_bit(EventBits_t bit);

static void button_task(void *arg) {
  Button button((uint8_t)(uintptr_t)arg); // Pin 10 for button
  ButtonState buttonState{false, ButtonEvent::None};
  ButtonState lastButtonState{false, ButtonEvent::None};

  for(;;) {
    buttonState.event = button.getEvent();
    buttonState.isPressed = button.isPressed();
    switch (current_system_mode->inputMode) {
      case InputMode::SimpleInput:
        // Haptics and Laser on press/release
        if(buttonState.isPressed != lastButtonState.isPressed) {
          if(xEventGroupGetBits(device_mode_event_group) & USING_HAPTICS)
            xQueueSend(haptics_queue, &buttonState, portMAX_DELAY); // Instant haptics feedback
          if(xEventGroupGetBits(device_mode_event_group) & USING_LASER)
            xQueueSend(laser_queue, &buttonState, portMAX_DELAY);   // Laser on
        }
        
        switch (buttonState.event) {
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
        if(buttonState.event != lastButtonState.event && xEventGroupGetBits(device_mode_event_group) & USING_HID)
          xQueueSend(hid_queue, &buttonState, portMAX_DELAY);

        break;
      case InputMode::Command:
        // Haptics feedback on event
        if(buttonState.event != lastButtonState.event && xEventGroupGetBits(device_mode_event_group) & USING_HAPTICS)
          xQueueSend(haptics_queue, &buttonState, portMAX_DELAY); // Command haptics feedback

        // System event group setup
        switch (buttonState.event) {
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
        // To be implemented
        break;
      default:
        break;
    }
    lastButtonState = buttonState;
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void button_task_start(QueueHandle_t haptics_q, QueueHandle_t laser_q, QueueHandle_t hid_q, EventGroupHandle_t eg, SystemMode* mode, uint8_t pin) {
  haptics_queue = haptics_q;
  laser_queue = laser_q;
  hid_queue = hid_q;
  device_mode_event_group = eg;
  current_system_mode = mode;

  xTaskCreate(
    button_task,
    "button_task",
    3072,
    (void*)(uintptr_t)pin,
    BUTTON_TASK_PRIORITY,
    &button_task_handle
  );
}

void toggle_event_group_bit(EventBits_t bit) {
  if(xEventGroupGetBits(device_mode_event_group) & bit) {
    xEventGroupClearBits(device_mode_event_group, bit);
  } else {
    xEventGroupSetBits(device_mode_event_group, bit);
  }
}
} // namespace ButtonTask