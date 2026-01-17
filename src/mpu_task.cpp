#include "mpu_task.h"

static MPUTask* mpu_task_instance = nullptr;

MPUTask::MPUTask()
  : mpu_task_handle(NULL),
    haptics_queue(nullptr),
    laser_queue(nullptr),
    hid_queue(nullptr),
    device_mode_event_group(nullptr),
    current_input_event(nullptr),
    current_system_mode(nullptr) {
}

void MPUTask::start(
  QueueHandle_t haptics_q, 
  QueueHandle_t laser_q, 
  QueueHandle_t hid_q, 
  EventGroupHandle_t eg, 
  InputEvent* input_event,
  SystemMode* mode
) {
  haptics_queue = haptics_q;
  laser_queue = laser_q;
  hid_queue = hid_q;
  device_mode_event_group = eg;
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
    switch (current_system_mode->inputMode) {
      case InputMode::SimpleInput:
        // No action needed
        break;
      case InputMode::Command:
        // No action needed
        break;
      case InputMode::MotionControl:
        if(xEventGroupGetBits(device_mode_event_group) & USING_MPU) {
          if(debounceDelay-- == 0) {
            debounceDelay = 20;
            if(mpu.getAccX() < -0.5) {
              current_input_event->buttonState.event = ButtonEvent::DoubleClick;
            } else if(mpu.getAccX() > 0.5) {
              current_input_event->buttonState.event = ButtonEvent::TripleClick;
            } else if(fabs(mpu.getAccZ()) > 1.5) {
              current_input_event->buttonState.event = ButtonEvent::SingleClick;
            }
          xQueueSend(hid_queue, current_input_event, portMAX_DELAY);
          }
        }
        break;
      default:
        break;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}