#include "hid_task.h"

namespace HIDTask {
static TaskHandle_t hid_task_handle = NULL;
static QueueHandle_t hid_queue;
static EventGroupHandle_t device_mode_event_group;
static SystemMode* current_system_mode;

void hid_execute(ButtonEvent evt, BLEHID& blehid);

static void hid_task(void *arg) {
  BLEHID blehid("Tu's Wand", "Tu123", 100);
  ButtonState buttonState;
  blehid.begin();
  for(;;) {
    if(xQueueReceive(hid_queue, &buttonState, portMAX_DELAY)) {
      hid_execute(buttonState.event, blehid);
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void hid_task_start(QueueHandle_t q, EventGroupHandle_t eg, SystemMode* mode) {
  hid_queue = q;
  device_mode_event_group = eg;
  current_system_mode = mode;

  xTaskCreate(
    hid_task,
    "hid_task",
    4096,
    NULL,
    HID_TASK_PRIORITY,
    &hid_task_handle
  );
}

void hid_execute(ButtonEvent evt, BLEHID& blehid) {
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
} // namespace HIDTask