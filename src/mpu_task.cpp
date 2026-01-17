#include "mpu_task.h"

namespace MPUTask {
static TaskHandle_t mpu_task_handle = NULL;
static QueueHandle_t haptics_queue;
static QueueHandle_t laser_queue;
static QueueHandle_t hid_queue;
static EventGroupHandle_t device_mode_event_group;
static InputEvent* current_input_event;
static SystemMode* current_system_mode;

static void mpu_task(void *arg) {
  MPU mpu;
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

void mpu_task_start(
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

  xTaskCreate(
    mpu_task,
    "mpu_task",
    4096,
    NULL,
    MPU_TASK_PRIORITY,
    &mpu_task_handle
  );
}
} // namespace MPUTask