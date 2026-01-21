#include "input_task/mpu_task.h"
#include "DeviceManager.h"

static MPUTask* mpu_task_instance = nullptr;

MPUTask::MPUTask()
  : mpu_task_handle(NULL),
    haptics_queue(nullptr),
    laser_queue(nullptr),
    hid_queue(nullptr),
    device_manager(nullptr),
    current_input_event(nullptr),
    current_system_mode(nullptr) {
}

void MPUTask::start(
  QueueHandle_t haptics_q, 
  QueueHandle_t laser_q, 
  QueueHandle_t hid_q, 
  DeviceManager* device_manager,
  InputEvent* input_event,
  SystemMode* mode
) {
  haptics_queue = haptics_q;
  laser_queue = laser_q;
  hid_queue = hid_q;
  this->device_manager = device_manager;
  current_input_event = input_event;
  current_system_mode = mode;

  mpu_task_instance = this;

  xTaskCreate(
    mpu_task_static,
    "mpu_task",
    4096,
    NULL,
    MPU_TASK_PRIORITY,
    &mpu_task_handle
  );
}

void MPUTask::mpu_task_static(void *arg) {
  if (mpu_task_instance) {
    mpu_task_instance->mpu_task_impl();
  }
}

void MPUTask::mpu_task_impl() {
  MPU mpu;
  while(!mpu.begin()) {
    Serial.println("[MPU Task]: Failed to initialize");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  uint32_t debounceDelay = 20; // milliseconds
  for(;;) {
    mpu.update();
    if(current_system_mode->inputMode == InputMode::MotionControl) {
      if(device_manager->isFeatureEnabled(USING_MPU)) {
        current_input_event->motionState = mpu.getMotionState();
        // Send motion event if any detected
        if(current_input_event->motionState.motionEvent != _lastInputEvent.motionState.motionEvent ||
           current_input_event->motionState.motionEvent == MotionEvent::RotateClockwise ||
           current_input_event->motionState.motionEvent == MotionEvent::RotateCounterClockwise) {
          if(millis() - _clock > CONTINUOUS_EVENT_THROTTLE) { // Throttle rapid events for fast output
            if(current_input_event->motionState.motionEvent == MotionEvent::RotateClockwise ||
              current_input_event->motionState.motionEvent == MotionEvent::RotateCounterClockwise) {
              _clock = millis();
            }
            if(device_manager->isFeatureEnabled(USING_HID)) {
              xQueueSend(hid_queue, current_input_event, portMAX_DELAY); // Send to HID task
            }
            if(device_manager->isFeatureEnabled(USING_HAPTICS)) {
              xQueueSend(haptics_queue, current_input_event, portMAX_DELAY); // Send to Haptics task
            }
          }
        }
        _lastInputEvent = *current_input_event;
      } else {
        // Clear motion event when MPU is disabled
        mpu.resetEventDetection();
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}