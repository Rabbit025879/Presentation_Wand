#include "haptics_task.h"

namespace HapticsTask {
static TaskHandle_t haptics_task_handle = NULL;
static QueueHandle_t haptics_queue;
static EventGroupHandle_t device_mode_event_group;
static SystemMode* current_system_mode;

bool execute_haptics(ButtonEvent evt, Haptics& haptics);

static void haptics_task(void *arg) {
  Haptics haptics((uint8_t)(uintptr_t)arg); // Pin A0 for vibrator motor
  InputEvent current_input_event;
  for(;;) {
    switch (current_system_mode->inputMode) {
      case InputMode::SimpleInput:
        if(xQueueReceive(haptics_queue, &current_input_event, portMAX_DELAY)) {
          if(current_input_event.buttonState.isPressed == true) haptics.turnOn(200);
          else                                                  haptics.turnOff();
        }
        break;
      case InputMode::Command:
        Serial.println("[Haptics Task]: Waiting for button event...");
        if(xQueueReceive(haptics_queue, &current_input_event, portMAX_DELAY)) {
          Serial.print("[Haptics Task]: Received button event ");
          Serial.println(static_cast<int>(current_input_event.buttonState.event));
          // If haptics is busy, wait until it finishes
          while(execute_haptics(current_input_event.buttonState.event, haptics) && current_input_event.buttonState.event != ButtonEvent::Hold) {
            vTaskDelay(pdMS_TO_TICKS(10));
          }  
        }
        break;
      case InputMode::MotionControl:
        // To be implemented
        break;
      default:
        break;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void haptics_task_start(
  QueueHandle_t q, 
  EventGroupHandle_t eg, 
  SystemMode* mode, 
  uint8_t pin
) {
  haptics_queue = q;
  device_mode_event_group = eg;
  current_system_mode = mode;

  xTaskCreate(
    haptics_task,
    "haptics_task",
    3072,
    (void*)(uintptr_t)pin,
    HAPTICS_TASK_PRIORITY,
    &haptics_task_handle
  );
}

bool execute_haptics(ButtonEvent evt, Haptics& haptics) {
  switch (evt) {
    case ButtonEvent::SingleClick:
      haptics.shortPulse(1, 200);
      break;
    case ButtonEvent::DoubleClick:
      haptics.shortPulse(2, 200);
      break;
    case ButtonEvent::TripleClick:
      haptics.shortPulse(3, 200);
      break;
    case ButtonEvent::SingleLongPress:
      haptics.longPulse(1, 200);
      break;
    case ButtonEvent::Hold:
      haptics.turnOn(200);
      break;
    case ButtonEvent::None:
      haptics.turnOff();
      break;
    default:
      Serial.println("[Haptics Task]: Unknown button event");
      break;
  }
  return haptics.isBusy();
}
} // namespace HapticsTask