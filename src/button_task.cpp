#include "button_task.h"

namespace ButtonTask {
static TaskHandle_t button_task_handle = NULL;
static QueueHandle_t haptics_queue;
static QueueHandle_t laser_queue;
static QueueHandle_t hid_queue;
static EventGroupHandle_t input_mode_event_group;

static void button_task(void *arg) {
  Button button((uint8_t)(uintptr_t)arg); // Pin 10 for button
  ButtonState buttonState;
  ButtonEvent lastButtonEvent = ButtonEvent::None;
  for(;;) {
    if(0) {
      if(button.isPressed()) {
      buttonState.isPressed = true;
      xQueueSend(haptics_queue, &buttonState, portMAX_DELAY);
      xQueueSend(laser_queue, &buttonState, portMAX_DELAY);
      }
      if(button.isReleased()) {
      buttonState.isPressed = false;
      xQueueSend(haptics_queue, &buttonState, portMAX_DELAY);
      xQueueSend(laser_queue, &buttonState, portMAX_DELAY);
      }
    }
    if(1) {
      buttonState.event = button.getEvent();
      if(buttonState.event != lastButtonEvent) {
        xQueueSend(haptics_queue, &buttonState, portMAX_DELAY);
        xQueueSend(laser_queue, &buttonState, portMAX_DELAY);
        xQueueSend(hid_queue, &buttonState, portMAX_DELAY);
      }
      lastButtonEvent = buttonState.event;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void button_task_start(QueueHandle_t haptics_q, QueueHandle_t laser_q, QueueHandle_t hid_q, EventGroupHandle_t eg, uint8_t pin) {
  haptics_queue = haptics_q;
  laser_queue = laser_q;
  hid_queue = hid_q;
  input_mode_event_group = eg;

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