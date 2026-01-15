#include "haptics_task.h"

namespace HapticsTask {
static TaskHandle_t haptics_task_handle = NULL;
static QueueHandle_t haptics_queue;
static EventGroupHandle_t input_mode_event_group;

bool execute_haptics(ButtonEvent evt, Haptics& haptics);

static void haptics_task(void *arg) {
  Haptics haptics((uint8_t)(uintptr_t)arg); // Pin A0 for vibrator motor
  ButtonState buttonState;
  for(;;) {
    if(0) {
      if(xQueueReceive(haptics_queue, &buttonState, portMAX_DELAY)) {
          // If haptics is busy, wait until it finishes
          if(buttonState.isPressed == true) haptics.turnOn(200);
          else                              haptics.turnOff();
      }
    }
    if(1) {
      if(xQueueReceive(haptics_queue, &buttonState, portMAX_DELAY)) {
        // If haptics is busy, wait until it finishes
        while(execute_haptics(buttonState.event, haptics) && buttonState.event != ButtonEvent::Hold) {
          vTaskDelay(pdMS_TO_TICKS(10));
        }  
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void haptics_task_start(QueueHandle_t q, EventGroupHandle_t eg, uint8_t pin) {
  haptics_queue = q;
  input_mode_event_group = eg;

  xTaskCreate(
    haptics_task,
    "haptics_task",
    3072,
    (void*)(uintptr_t)pin,
    4,
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
    case ButtonEvent::LongPress:
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