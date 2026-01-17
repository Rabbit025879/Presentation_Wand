#include "hid_task.h"

static HIDTask* hid_task_instance = nullptr;

HIDTask::HIDTask()
  : hid_task_handle(NULL),
    hid_queue(nullptr),
    device_mode_event_group(nullptr),
    current_system_mode(nullptr) {
}

void HIDTask::start(
  QueueHandle_t q, 
  EventGroupHandle_t eg, 
  SystemMode* mode
) {
  hid_queue = q;
  device_mode_event_group = eg;
  current_system_mode = mode;

  hid_task_instance = this;

  xTaskCreate(
    hid_task_static,
    "hid_task",
    4096,
    NULL,
    HID_TASK_PRIORITY,
    &hid_task_handle
  );
}

void HIDTask::hid_task_static(void *arg) {
  if (hid_task_instance) {
    hid_task_instance->hid_task_impl();
  }
}

void HIDTask::hid_task_impl() {
  BLEHID blehid("Tu's Wand", "Tu123", 100);
  InputEvent current_input_event;
  blehid.begin();
  for(;;) {
    if(xQueueReceive(hid_queue, &current_input_event, portMAX_DELAY)) {
      hid_execute(current_input_event.buttonState.event, blehid);
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void HIDTask::hid_execute(ButtonEvent evt, BLEHID& blehid) {
  switch (evt) {
    case ButtonEvent::SingleClick:
      blehid.getKeyboard().write(KEY_MEDIA_PLAY_PAUSE);
      break;
    case ButtonEvent::DoubleClick:
      blehid.getKeyboard().write(KEY_MEDIA_NEXT_TRACK);
      break;
    case ButtonEvent::TripleClick:
      blehid.getKeyboard().write(KEY_MEDIA_PREVIOUS_TRACK);
      break;
    case ButtonEvent::SingleLongPress:
      blehid.getKeyboard().write(KEY_MEDIA_MUTE);
      break;
    default:
      break;
  }
}