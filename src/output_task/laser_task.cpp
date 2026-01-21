#include "output_task/laser_task.h"
#include "DeviceManager.h"
#include "Utils.h"

static LaserTask* laser_task_instance = nullptr;

LaserTask::LaserTask()
  : laser_task_handle(NULL),
    laser_queue(nullptr),
    device_manager(nullptr),
    current_system_mode(nullptr) {
}

void LaserTask::start(
  QueueHandle_t q, 
  DeviceManager* device_manager,
  SystemMode* mode
) {
  laser_queue = q;
  this->device_manager = device_manager;
  current_system_mode = mode;

  laser_task_instance = this;

  xTaskCreate(
    laser_task_static,
    "laser_task",
    3072,
    NULL,
    LASER_TASK_PRIORITY,
    &laser_task_handle
  );
}

void LaserTask::laser_task_static(void *arg) {
  if (laser_task_instance) {
    laser_task_instance->laser_task_impl();
  }
}

void LaserTask::laser_task_impl() {
  Laser laser;
  InputEvent current_input_event;
  
  for(;;) {
    if(xQueueReceive(laser_queue, &current_input_event, portMAX_DELAY)) {
      // Laser only active in presentation mode and non-command input modes
      if((current_system_mode->inputMode == InputMode::SimpleInput ||
          current_system_mode->inputMode == InputMode::MotionControl) &&
         current_system_mode->functionMode == FunctionMode::Presentation) {
        if(current_input_event.buttonStates.pointerButton.isPressed) {
          laser.turnOn(device_manager->getLaserIntensity());
        } else {
          laser.turnOff();
        }
      } else {
        laser.turnOff();
      }
    } 
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}