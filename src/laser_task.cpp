#include "laser_task.h"

static LaserTask* laser_task_instance = nullptr;

LaserTask::LaserTask()
  : laser_task_handle(NULL),
    laser_queue(nullptr),
    device_mode_event_group(nullptr),
    current_system_mode(nullptr) {
}

void LaserTask::start(
  QueueHandle_t q, 
  EventGroupHandle_t eg, 
  SystemMode* mode, 
  uint8_t pin
) {
  laser_queue = q;
  device_mode_event_group = eg;
  current_system_mode = mode;

  laser_task_instance = this;

  xTaskCreate(
    laser_task_static,
    "laser_task",
    3072,
    (void*)(uintptr_t)pin,
    LASER_TASK_PRIORITY,
    &laser_task_handle
  );
}

void LaserTask::laser_task_static(void *arg) {
  if (laser_task_instance) {
    laser_task_instance->laser_task_impl((uint8_t)(uintptr_t)arg);
  }
}

void LaserTask::laser_task_impl(uint8_t pin) {
  Laser laser(pin);
  InputEvent current_input_event;
  for(;;) {
    if(current_system_mode->inputMode == InputMode::SimpleInput) {
      if(xQueueReceive(laser_queue, &current_input_event, portMAX_DELAY)) {
        if(current_input_event.buttonState.isPressed == true) laser.turnOn(150);
        else                                                  laser.turnOff();
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

bool LaserTask::execute_laser(ButtonEvent evt, Laser& laser) {
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
    case ButtonEvent::SingleLongPress:
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