#include "output_task/haptics_task.h"
#include "DeviceManager.h"

static HapticsTask* haptics_task_instance = nullptr;

HapticsTask::HapticsTask()
  : haptics_task_handle(NULL),
    haptics_queue(nullptr),
    device_manager(nullptr),
    current_system_mode(nullptr) {
}

void HapticsTask::start(
  QueueHandle_t q, 
  DeviceManager* device_manager,
  SystemMode* mode
) {
  haptics_queue = q;
  this->device_manager = device_manager;
  current_system_mode = mode;

  haptics_task_instance = this;

  xTaskCreate(
    haptics_task_static,
    "haptics_task",
    3072,
    NULL,
    HAPTICS_TASK_PRIORITY,
    &haptics_task_handle
  );
}

void HapticsTask::haptics_task_static(void *arg) {
  if (haptics_task_instance) {
    haptics_task_instance->haptics_task_impl();
  }
}

void HapticsTask::haptics_task_impl() {
  Haptics haptics;
  InputEvent current_input_event;
  for(;;) {
    switch (current_system_mode->inputMode) {
      case InputMode::SimpleInput:
        if(xQueueReceive(haptics_queue, &current_input_event, portMAX_DELAY)) {
          if(current_input_event.buttonStates.pointerButton.isPressed == true)  haptics.turnOn(device_manager->getHapticsIntensity());
          else                                                                  haptics.turnOff();
        }
        break;

      case InputMode::MotionControl:
        // Send haptics if motion event detected
        if(xQueueReceive(haptics_queue, &current_input_event, portMAX_DELAY)) {
          // If haptics is busy, wait until it finishes
          if(current_input_event.motionState.motionEvent != MotionEvent::None) {
            do {
              haptics.shortPulse(1, device_manager->getHapticsIntensity());
              vTaskDelay(pdMS_TO_TICKS(10));
            } while(haptics.isBusy());
          }
          if(current_input_event.buttonStates.pointerButton.event == ButtonEvent::DummyEvent) {
            do {
              haptics.longPulse(1, device_manager->getHapticsIntensity());
              vTaskDelay(pdMS_TO_TICKS(10));
            } while(haptics.isBusy());
          }
        }
        break;

      case InputMode::Command:
        if(xQueueReceive(haptics_queue, &current_input_event, portMAX_DELAY)) {
          // If haptics is busy, wait until it finishes
          if(current_input_event.buttonStates.pointerButton.event == ButtonEvent::DummyEvent) {
            do {
              haptics.emitPulses(
                device_manager->getHapticsIntensity(), 
                10, // duration (100ms units)
                2, // cycles
                1  // pause (100ms units)
              );
              vTaskDelay(pdMS_TO_TICKS(10));
            } while(haptics.isBusy());
          }
          if(current_input_event.buttonStates.thumbsDownButton.event == ButtonEvent::DummyEvent) {
            do {
              haptics.longPulse(1, device_manager->getHapticsIntensity());
              vTaskDelay(pdMS_TO_TICKS(10));
            } while(haptics.isBusy());
          }
        }
        break;

      default:
        break;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}