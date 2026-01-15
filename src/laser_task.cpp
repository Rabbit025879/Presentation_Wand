#include "laser_task.h"

namespace LaserTask {
static TaskHandle_t laser_task_handle = NULL;
static QueueHandle_t laser_queue;
static EventGroupHandle_t input_mode_event_group;

bool execute_laser(ButtonEvent evt, Laser& laser);

static void laser_task(void *arg) {
  Laser laser((uint8_t)(uintptr_t)arg); // Pin A0 for laser device
  ButtonState buttonState;
  for(;;) {
    if(0) {
      if(xQueueReceive(laser_queue, &buttonState, portMAX_DELAY)) {
      // If laser is busy, wait until it finishes
      if(buttonState.isPressed == true) laser.turnOn(150);
      else                              laser.turnOff();
      }
    }
    if(1) {
      if(xQueueReceive(laser_queue, &buttonState, portMAX_DELAY)) {
        // If laser is busy, wait until it finishes
        while(execute_laser(buttonState.event, laser) && buttonState.event != ButtonEvent::Hold) {
          vTaskDelay(pdMS_TO_TICKS(10));
        }  
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void laser_task_start(QueueHandle_t q, EventGroupHandle_t eg, uint8_t pin) {
  laser_queue = q;
  input_mode_event_group = eg;

  xTaskCreate(
    laser_task,
    "laser_task",
    3072,
    (void*)(uintptr_t)pin,
    5,
    &laser_task_handle
  );
}

bool execute_laser(ButtonEvent evt, Laser& laser) {
  switch (evt) {
    case ButtonEvent::SingleClick:
      laser.shortPulse(1, 150);
      break;
    case ButtonEvent::DoubleClick:
      laser.shortPulse(2, 150);
      break;
    case ButtonEvent::TripleClick:
      laser.shortPulse(3, 150);
      break;
    case ButtonEvent::LongPress:
      laser.longPulse(1, 150);
      break;
    case ButtonEvent::Hold:
      laser.turnOn(150);
      break;
    case ButtonEvent::None:
      laser.turnOff();
      break;
    default:
      Serial.println("[Laser Task]: Unknown button event");
      break;
  }
  return laser.isBusy();
}
} // namespace LaserTask