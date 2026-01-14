#include "ui_task.h"

namespace HapticsTask {
static TaskHandle_t haptics_task_handle = NULL;
static QueueHandle_t haptics_queue;

static void haptics_task(void *arg) {
  Haptics haptics((uint8_t)(uintptr_t)arg); // Pin A0 for vibrator motor
  ButtonEvent evt;
  for(;;) {
    if(xQueueReceive(haptics_queue, &evt, portMAX_DELAY)) {
      // If haptics is busy, wait until it finishes
      while(execute_haptics(evt, haptics) && evt != ButtonEvent::Hold) {
        vTaskDelay(pdMS_TO_TICKS(10));
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void haptics_task_start(QueueHandle_t q, uint8_t pin) {
  haptics_queue = q;

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
      haptics.shortVibrate(1, 200);
      break;
    case ButtonEvent::DoubleClick:
      haptics.shortVibrate(2, 200);
      break;
    case ButtonEvent::TripleClick:
      haptics.shortVibrate(3, 200);
      break;
    case ButtonEvent::LongPress:
      haptics.longVibrate(1, 200);
      break;
    case ButtonEvent::Hold:
      haptics.startVibrate(150);
      break;
    case ButtonEvent::None:
      haptics.stopVibrate();
      break;
    default:
      Serial.println("[Haptics Task]: Unknown button event");
      break;
  }
  return haptics.isBusy();
}
} // namespace HapticsTask

namespace ButtonTask {
static TaskHandle_t button_task_handle = NULL;
static QueueHandle_t button_queue;

static void button_task(void *arg) {
  Button button((uint8_t)(uintptr_t)arg); // Pin 10 for button
  ButtonEvent evt;
  ButtonEvent lastButtonEvent = ButtonEvent::None;
  for(;;) {
    evt = button.getEvent();
    if(evt != lastButtonEvent) {
      if(xQueueSend(button_queue, &evt, portMAX_DELAY)) {
      } else {
        Serial.println("[Button Task] Failed to send button event to queue !!");
      }
    }
    lastButtonEvent = evt;
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void button_task_start(QueueHandle_t q, uint8_t pin) {
  button_queue = q;

  xTaskCreate(
    button_task,
    "button_task",
    3072,
    (void*)(uintptr_t)pin,
    5,
    &button_task_handle
  );
}
} // namespace ButtonTask