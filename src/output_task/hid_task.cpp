#include "output_task/hid_task.h"
#include "Utils.h"

static HIDTask* hid_task_instance = nullptr;

HIDTask::HIDTask()
  : hid_task_handle(NULL),
    hid_queue(nullptr),
    device_manager(nullptr),
    current_system_mode(nullptr) {
}

void HIDTask::start(
  QueueHandle_t q, 
  DeviceManager* device_manager,
  SystemMode* mode
) {
  hid_queue = q;
  this->device_manager = device_manager;
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
    switch (current_system_mode->inputMode) {
      case InputMode::SimpleInput:
        if(xQueueReceive(hid_queue, &current_input_event, portMAX_DELAY)) {
          if(current_system_mode->functionMode == FunctionMode::Presentation) {
            hid_execute_presentation_simple(current_input_event, blehid);
          } else if(current_system_mode->functionMode == FunctionMode::MediaControl) {
            hid_execute_media_simple(current_input_event, blehid);
          }
        }
        break;

      case InputMode::MotionControl:
        if(xQueueReceive(hid_queue, &current_input_event, portMAX_DELAY)) {
          if(current_system_mode->functionMode == FunctionMode::Presentation) {
            hid_execute_presentation_motion(current_input_event, blehid);
          } else if(current_system_mode->functionMode == FunctionMode::MediaControl) {
            hid_execute_media_motion(current_input_event, blehid);
          }
        }
        break;
      
      case InputMode::Command:
        break;
      
      default:
        break;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

// Presentation mode - Simple input
void HIDTask::hid_execute_presentation_simple(InputEvent& evt, BLEHID& blehid) {
  // Thumbs Up button actions
  switch(evt.buttonStates.thumbsUpButton.event) {
    case ButtonEvent::SingleClick:
      blehid.getKeyboard().write(KEY_RIGHT_ARROW);
      DEBUG_PRINTLN("Presentation: Next Page");
      break;
    case ButtonEvent::DoubleClick:
      blehid.getKeyboard().write(KEY_MEDIA_VOLUME_UP);
      DEBUG_PRINTLN("Presentation: Volume Up");
      break;
    case ButtonEvent::TripleClick:
      blehid.getKeyboard().write(KEY_F11);
      DEBUG_PRINTLN("Presentation: Full Screen");
      break;
    default:
      break;
  }

  // Thumbs Down button actions
  switch(evt.buttonStates.thumbsDownButton.event) {
    case ButtonEvent::SingleClick:
      blehid.getKeyboard().write(KEY_LEFT_ARROW);
      DEBUG_PRINTLN("Presentation: Previous Page");
      break;
    case ButtonEvent::DoubleClick:
      blehid.getKeyboard().write(KEY_MEDIA_VOLUME_DOWN);
      DEBUG_PRINTLN("Presentation: Volume Down");
      break;
    case ButtonEvent::TripleClick:
      blehid.getKeyboard().write(' ');
      DEBUG_PRINTLN("Presentation: White Space");
      break;
    default:
      break;
  }
}

// Presentation mode - Motion control
void HIDTask::hid_execute_presentation_motion(InputEvent& evt, BLEHID& blehid) {
  // Thumbs Up button actions
  switch(evt.buttonStates.thumbsUpButton.event) {
    case ButtonEvent::SingleClick:
      blehid.getKeyboard().write(KEY_F11);
      DEBUG_PRINTLN("Presentation Motion: Full Screen");
      break;
    case ButtonEvent::DoubleClick:
      blehid.getKeyboard().write(' ');
      DEBUG_PRINTLN("Presentation Motion: White Space");
      break;
    case ButtonEvent::TripleClick:
      break;
    default:
      break;
  }

  // Motion events
  switch(evt.motionState.motionEvent) {
    case MotionEvent::FlickUp:
      blehid.getKeyboard().write(KEY_RIGHT_ARROW);
      DEBUG_PRINTLN("Presentation Motion: Flick Up -> Next Page");
      break;
    case MotionEvent::FlickDown:
      blehid.getKeyboard().write(KEY_LEFT_ARROW);
      DEBUG_PRINTLN("Presentation Motion: Flick Down -> Previous Page");
      break;
    case MotionEvent::FlickRight:
      blehid.getKeyboard().write(KEY_RIGHT_ARROW);
      DEBUG_PRINTLN("Presentation Motion: Flick Right -> Next Page");
      break;
    case MotionEvent::FlickLeft:
      blehid.getKeyboard().write(KEY_LEFT_ARROW);
      DEBUG_PRINTLN("Presentation Motion: Flick Left -> Previous Page");
      break;
    case MotionEvent::RotateClockwise:
      blehid.getKeyboard().write(KEY_RIGHT_ARROW);
      DEBUG_PRINTLN("Presentation Motion: Rotate CW -> Next Page");
      break;
    case MotionEvent::RotateCounterClockwise:
      blehid.getKeyboard().write(KEY_LEFT_ARROW);
      DEBUG_PRINTLN("Presentation Motion: Rotate CCW -> Previous Page");
      break;
    default:
      break;
  }
}

// Media control mode - Simple input
void HIDTask::hid_execute_media_simple(InputEvent& evt, BLEHID& blehid) {
  // Pointer button actions
  switch(evt.buttonStates.pointerButton.event) {
    case ButtonEvent::SingleClick:
      blehid.getKeyboard().write(KEY_MEDIA_PLAY_PAUSE);
      DEBUG_PRINTLN("Media: Play/Pause");
      break;
    case ButtonEvent::DoubleClick:
      blehid.getKeyboard().write(KEY_MEDIA_NEXT_TRACK);
      DEBUG_PRINTLN("Media: Next Song");
      break;
    case ButtonEvent::TripleClick:
      blehid.getKeyboard().write(KEY_MEDIA_PREVIOUS_TRACK);
      DEBUG_PRINTLN("Media: Previous Song");
      break;
    default:
      break;
  }

  // Thumbs Up button: Volume up
  if(evt.buttonStates.thumbsUpButton.event == ButtonEvent::SingleClick) {
    blehid.getKeyboard().write(KEY_MEDIA_VOLUME_UP);
    DEBUG_PRINTLN("Media: Volume Up");
  }

  // Thumbs Down button: Volume down
  if(evt.buttonStates.thumbsDownButton.event == ButtonEvent::SingleClick) {
    blehid.getKeyboard().write(KEY_MEDIA_VOLUME_DOWN);
    DEBUG_PRINTLN("Media: Volume Down");
  }
}

// Media control mode - Motion control
void HIDTask::hid_execute_media_motion(InputEvent& evt, BLEHID& blehid) {
  // Pointer button: Play/Pause
  if(evt.buttonStates.pointerButton.event == ButtonEvent::SingleClick) {
    blehid.getKeyboard().write(KEY_MEDIA_PLAY_PAUSE);
    DEBUG_PRINTLN("Media Motion: Play/Pause");
  }

  // Thumbs Up button: Volume up
  if(evt.buttonStates.thumbsUpButton.event == ButtonEvent::SingleClick) {
    blehid.getKeyboard().write(KEY_MEDIA_VOLUME_UP);
    DEBUG_PRINTLN("Media Motion: Volume Up");
  }

  // Thumbs Down button: Volume down
  if(evt.buttonStates.thumbsDownButton.event == ButtonEvent::SingleClick) {
    blehid.getKeyboard().write(KEY_MEDIA_VOLUME_DOWN);
    DEBUG_PRINTLN("Media Motion: Volume Down");
  }

  // Motion events
  switch(evt.motionState.motionEvent) {
    case MotionEvent::FlickUp:
      blehid.getKeyboard().write(KEY_MEDIA_NEXT_TRACK);
      DEBUG_PRINTLN("Media Motion: Flick Up -> Next Song");
      break;
    case MotionEvent::FlickDown:
      blehid.getKeyboard().write(KEY_MEDIA_PREVIOUS_TRACK);
      DEBUG_PRINTLN("Media Motion: Flick Down -> Previous Song");
      break;
    case MotionEvent::FlickRight:
      blehid.getKeyboard().write(KEY_MEDIA_NEXT_TRACK);
      DEBUG_PRINTLN("Media Motion: Flick Right -> Next Song");
      break;
    case MotionEvent::FlickLeft:
      blehid.getKeyboard().write(KEY_MEDIA_PREVIOUS_TRACK);
      DEBUG_PRINTLN("Media Motion: Flick Left -> Previous Song");
      break;
    case MotionEvent::RotateClockwise:
      blehid.getKeyboard().write(KEY_MEDIA_VOLUME_UP);
      DEBUG_PRINTLN("Media Motion: Rotate CW -> Volume Up");
      break;
    case MotionEvent::RotateCounterClockwise:
      blehid.getKeyboard().write(KEY_MEDIA_VOLUME_DOWN);
      DEBUG_PRINTLN("Media Motion: Rotate CCW -> Volume Down");
      break;
    default:
      break;
  }
}