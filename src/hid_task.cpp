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
        // No HID action needed in command mode
        break;
      
      default:
        break;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

// Presentation Mode - Simple IO
void HIDTask::hid_execute_presentation_simple(InputEvent& evt, BLEHID& blehid) {
  // ** Pointer Button
  // No action assigned for pointer button in presentation simple mode

  // ** Thumbs Up Button
  switch(evt.buttonStates.thumbsUpButton.event) {
    case ButtonEvent::SingleClick:
      // Next page (arrow right)
      blehid.getKeyboard().write(KEY_RIGHT_ARROW);
      Serial.println("Presentation: Next Page");
      break;
    case ButtonEvent::DoubleClick:
      // Volume up
      blehid.getKeyboard().write(KEY_MEDIA_VOLUME_UP);
      Serial.println("Presentation: Volume Up");
      break;
    case ButtonEvent::TripleClick:
      // Full screen (F11)
      blehid.getKeyboard().write(KEY_F11);
      Serial.println("Presentation: Full Screen");
      break;
    default:
      break;
  }

  // ** Thumbs Down Button
  switch(evt.buttonStates.thumbsDownButton.event) {
    case ButtonEvent::SingleClick:
      // Previous page (arrow left)
      blehid.getKeyboard().write(KEY_LEFT_ARROW);
      Serial.println("Presentation: Previous Page");
      break;
    case ButtonEvent::DoubleClick:
      // Volume down
      blehid.getKeyboard().write(KEY_MEDIA_VOLUME_DOWN);
      Serial.println("Presentation: Volume Down");
      break;
    case ButtonEvent::TripleClick:
      // White space
      blehid.getKeyboard().write(' ');
      Serial.println("Presentation: White Space");
      break;
    default:
      break;
  }
}

// Presentation Mode - Motion Control
void HIDTask::hid_execute_presentation_motion(InputEvent& evt, BLEHID& blehid) {
  // ** Pointer Button
  // No action assigned for pointer button in presentation motion mode

  // ** Thumbs Up Button
  switch(evt.buttonStates.thumbsUpButton.event) {
    case ButtonEvent::SingleClick:
      // Full screen (F11)
      blehid.getKeyboard().write(KEY_F11);
      Serial.println("Presentation Motion: Full Screen");
      break;
    case ButtonEvent::DoubleClick:
      // White space
      blehid.getKeyboard().write(' ');
      Serial.println("Presentation Motion: White Space");
      break;
    case ButtonEvent::TripleClick:
      // Lock motion detect (handled by button task)
      break;
    default:
      break;
  }

  // Motion events
  switch(evt.motionState.motionEvent) {
    case MotionEvent::FlickUp:
      // Next page
      blehid.getKeyboard().write(KEY_RIGHT_ARROW);
      Serial.println("Presentation Motion: Flick Up -> Next Page");
      break;
    case MotionEvent::FlickDown:
      // Previous page
      blehid.getKeyboard().write(KEY_LEFT_ARROW);
      Serial.println("Presentation Motion: Flick Down -> Previous Page");
      break;
    case MotionEvent::FlickRight:
      // Next page
      blehid.getKeyboard().write(KEY_RIGHT_ARROW);
      Serial.println("Presentation Motion: Flick Right -> Next Page");
      break;
    case MotionEvent::FlickLeft:
      // Previous page
      blehid.getKeyboard().write(KEY_LEFT_ARROW);
      Serial.println("Presentation Motion: Flick Left -> Previous Page");
      break;
    case MotionEvent::RotateClockwise:
      // Continuously next page (arrow right)
      blehid.getKeyboard().write(KEY_RIGHT_ARROW);
      Serial.println("Presentation Motion: Rotate CW -> Next Page");
      break;
    case MotionEvent::RotateCounterClockwise:
      // Continuously previous page (arrow left)
      blehid.getKeyboard().write(KEY_LEFT_ARROW);
      Serial.println("Presentation Motion: Rotate CCW -> Previous Page");
      break;
    default:
      break;
  }
}

// Media Control Mode - Simple IO
void HIDTask::hid_execute_media_simple(InputEvent& evt, BLEHID& blehid) {
  // ** Pointer Button
  switch(evt.buttonStates.pointerButton.event) {
    case ButtonEvent::SingleClick:
      // Play/Pause
      blehid.getKeyboard().write(KEY_MEDIA_PLAY_PAUSE);
      Serial.println("Media: Play/Pause");
      break;
    case ButtonEvent::DoubleClick:
      // Next song
      blehid.getKeyboard().write(KEY_MEDIA_NEXT_TRACK);
      Serial.println("Media: Next Song");
      break;
    case ButtonEvent::TripleClick:
      // Previous song
      blehid.getKeyboard().write(KEY_MEDIA_PREVIOUS_TRACK);
      Serial.println("Media: Previous Song");
      break;
    default:
      break;
  }

  // ** Thumbs Up Button
  if(evt.buttonStates.thumbsUpButton.event == ButtonEvent::SingleClick) {
    // Volume up
    blehid.getKeyboard().write(KEY_MEDIA_VOLUME_UP);
    Serial.println("Media: Volume Up");
  }

  // ** Thumbs Down Button
  if(evt.buttonStates.thumbsDownButton.event == ButtonEvent::SingleClick) {
    // Volume down
    blehid.getKeyboard().write(KEY_MEDIA_VOLUME_DOWN);
    Serial.println("Media: Volume Down");
  }
}

// Media Control Mode - Motion Control
void HIDTask::hid_execute_media_motion(InputEvent& evt, BLEHID& blehid) {
  // ** Pointer Button
  if(evt.buttonStates.pointerButton.event == ButtonEvent::SingleClick) {
    // Play/Pause
    blehid.getKeyboard().write(KEY_MEDIA_PLAY_PAUSE);
    Serial.println("Media Motion: Play/Pause");
  }

  // ** Thumbs Up Button
  if(evt.buttonStates.thumbsUpButton.event == ButtonEvent::SingleClick) {
    // Volume up
    blehid.getKeyboard().write(KEY_MEDIA_VOLUME_UP);
    Serial.println("Media Motion: Volume Up");
  }

  // ** Thumbs Down Button
  if(evt.buttonStates.thumbsDownButton.event == ButtonEvent::SingleClick) {
    // Volume down
    blehid.getKeyboard().write(KEY_MEDIA_VOLUME_DOWN);
    Serial.println("Media Motion: Volume Down");
  }

  // Motion events
  switch(evt.motionState.motionEvent) {
    case MotionEvent::FlickUp:
      // Next song
      blehid.getKeyboard().write(KEY_MEDIA_NEXT_TRACK);
      Serial.println("Media Motion: Flick Up -> Next Song");
      break;
    case MotionEvent::FlickDown:
      // Previous song
      blehid.getKeyboard().write(KEY_MEDIA_PREVIOUS_TRACK);
      Serial.println("Media Motion: Flick Down -> Previous Song");
      break;
    case MotionEvent::FlickRight:
      // Next song
      blehid.getKeyboard().write(KEY_MEDIA_NEXT_TRACK);
      Serial.println("Media Motion: Flick Right -> Next Song");
      break;
    case MotionEvent::FlickLeft:
      // Previous song
      blehid.getKeyboard().write(KEY_MEDIA_PREVIOUS_TRACK);
      Serial.println("Media Motion: Flick Left -> Previous Song");
      break;
    case MotionEvent::RotateClockwise:
      // Volume up
      blehid.getKeyboard().write(KEY_MEDIA_VOLUME_UP);
      Serial.println("Media Motion: Rotate CW -> Volume Up");
      break;
    case MotionEvent::RotateCounterClockwise:
      // Volume down
      blehid.getKeyboard().write(KEY_MEDIA_VOLUME_DOWN);
      Serial.println("Media Motion: Rotate CCW -> Volume Down");
      break;
    default:
      break;
  }
}