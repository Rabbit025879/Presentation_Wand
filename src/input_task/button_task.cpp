#include "input_task/button_task.h"
#include "SettingsManager.h"

// Static instance pointer for FreeRTOS callback
static ButtonTask* button_task_instance = nullptr;
static SettingsManager* global_settings_manager = nullptr;

ButtonTask::ButtonTask()
  : button_task_handle(NULL),
    haptics_queue(nullptr),
    laser_queue(nullptr),
    hid_queue(nullptr),
    device_mode_event_group(nullptr),
    current_input_event(nullptr),
    current_system_mode(nullptr) {
}

void ButtonTask::start(
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

  button_task_instance = this;

  xTaskCreate(
    button_task_static,
    "button_task",
    3072,
    NULL,
    BUTTON_TASK_PRIORITY,
    &button_task_handle
  );
}

void ButtonTask::button_task_static(void *arg) {
  if (button_task_instance) {
    button_task_instance->button_task_impl();
  }
}

void ButtonTask::button_task_impl() {
  MultiButton multi_button;
  DeviceSettings device_settings;
  SettingsManager settings_manager;
  global_settings_manager = &settings_manager;
  
  bool in_settings_menu = false;
  bool in_tuning_mode = false;
  uint8_t tuning_target = 0; // 0=haptics, 1=laser, 2=led

  for(;;) {
    // ** Update button states
    current_input_event->buttonStates = multi_button.getState();  // ! Do not change motionState here, it will be updated by MPU task
    
    // ** Functionality Mode and Input Mode Switching
    // ** Enter Command Mode
    // Check for Command Mode entry: Hold all three buttons
    if(multi_button.allThreeButtonsHold()) {
      // Transition to Command Mode if not already there
      if(current_system_mode->inputMode != InputMode::Command) {
        current_system_mode->inputMode = InputMode::Command;
        in_settings_menu = true;
        Serial.println("Entered Command Mode - OTA/Settings Mode");
        device_settings = DeviceSettings(); // Reset settings
        settings_manager.printCurrentSettings();
      } else {
        // Restart Device
        Serial.println("Restarting Device...");
        current_input_event->buttonStates.pointerButton.isPressed = true; // Dummy event to trigger haptics
        xQueueSend(haptics_queue, current_input_event, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_restart();
      }
    }
    
    // ** Switch Function Mode
    // Check for mode switch: Hold Thumbs Up/Down simultaneously
    if(multi_button.bothThumbButtonsHold() && current_system_mode->inputMode != InputMode::Command) {
      // Switch function mode
      if(current_system_mode->functionMode == FunctionMode::Presentation) {
        current_system_mode->functionMode = FunctionMode::MediaControl;
        Serial.println("Switched to Media Control Mode");
      } else {  
        current_system_mode->functionMode = FunctionMode::Presentation;
        Serial.println("Switched to Presentation Mode");
      }
    }

    // ** Handle Input Modes
    switch (current_system_mode->inputMode) {
      case InputMode::SimpleInput:
        send_queue(laser_queue, USING_LASER);
        send_queue(hid_queue, USING_HID);
        _lastInputMode = InputMode::SimpleInput;
        break;

      case InputMode::MotionControl:
        motion_control_specific();
        send_queue(laser_queue, USING_LASER);
        send_queue(hid_queue, USING_HID);
        _lastInputMode = InputMode::MotionControl;
        break;

      case InputMode::Command:
        handle_command_mode(device_settings, settings_manager, in_settings_menu, in_tuning_mode, tuning_target);
        break;

      default:
        break;
    }
    
    _lastInputEvent = *current_input_event;
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

// ** Motion Control Mode
void ButtonTask::motion_control_specific() { 
  // ** Thumbs Up Button: Special function - Toggle motion detect lock
  SettingsManager& sm = *global_settings_manager;
  if(current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::TripleClick &&
     _lastInputEvent.buttonStates.thumbsUpButton.event != ButtonEvent::TripleClick) {
    sm.toggleMotionDetectLock();
    Serial.print("Motion Detect Lock: ");
    Serial.println(sm.isMotionDetectLocked() ? "ON" : "OFF");
    return;
  }

  // ** Thumbs Down Button: Enable/Disable motion detect
  if(current_input_event->buttonStates.thumbsDownButton.isPressed != 
     _lastInputEvent.buttonStates.thumbsDownButton.isPressed) {
    if(current_input_event->buttonStates.thumbsDownButton.isPressed) {
      xEventGroupSetBits(device_mode_event_group, USING_MPU);
      Serial.println("Motion Detect: ON");
    } else {
      xEventGroupClearBits(device_mode_event_group, USING_MPU);
      current_input_event->motionState = MotionState(); // Clear motion event
      Serial.println("Motion Detect: OFF");
    }
  }
}

void ButtonTask::handle_command_mode(DeviceSettings& settings, SettingsManager& sm, bool& in_settings, bool& in_tuning, uint8_t& tuning_target) {
  // Pointer Button: Single Click to switch Function Mode
  if(current_input_event->buttonStates.pointerButton.event == ButtonEvent::SingleClick &&
     _lastInputEvent.buttonStates.pointerButton.event != ButtonEvent::SingleClick) {
    if(_lastInputMode == InputMode::SimpleInput) {
      current_system_mode->inputMode = InputMode::MotionControl;
      Serial.println("Switched to Motion Control Mode");
    } else {
      current_system_mode->inputMode = InputMode::SimpleInput;
      Serial.println("Switched to Simple Input Mode");
    }
  }

  // Pointer Button: Double Click to toggle haptics
  if(current_input_event->buttonStates.pointerButton.event == ButtonEvent::DoubleClick &&
     _lastInputEvent.buttonStates.pointerButton.event != ButtonEvent::DoubleClick) {
    sm.toggleHaptics();
    Serial.print("Haptics: ");
    Serial.println(sm.isHapticsEnabled() ? "ON" : "OFF");
  }

  // Pointer Button: Triple Click to toggle LED
  if(current_input_event->buttonStates.pointerButton.event == ButtonEvent::TripleClick &&
     _lastInputEvent.buttonStates.pointerButton.event != ButtonEvent::TripleClick) {
    sm.toggleLed();
    Serial.print("LED: ");
    Serial.println(sm.isLedEnabled() ? "ON" : "OFF");
  }

  // Thumbs Up Button: Single Click to toggle/activate haptics
  if(current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::SingleClick &&
     _lastInputEvent.buttonStates.thumbsUpButton.event != ButtonEvent::SingleClick) {
    if(in_tuning) {
      sm.increaseSetting(tuning_target, 10);
    } else {
      sm.toggleHaptics();
      Serial.print("Haptics: ");
      Serial.println(sm.isHapticsEnabled() ? "ON" : "OFF");
    }
  }

  // Thumbs Up Button: Double Click to toggle/activate laser
  if(current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::DoubleClick &&
     _lastInputEvent.buttonStates.thumbsUpButton.event != ButtonEvent::DoubleClick) {
    if(in_tuning) {
      // Continue tuning
    } else {
      sm.toggleLaser();
      Serial.print("Laser: ");
      Serial.println(sm.isLaserEnabled() ? "ON" : "OFF");
    }
  }

  // Thumbs Up Button: Triple Click to toggle/activate BLE HID
  if(current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::TripleClick &&
     _lastInputEvent.buttonStates.thumbsUpButton.event != ButtonEvent::TripleClick) {
    if(!in_tuning) {
      toggle_event_group_bit(USING_HID);
      Serial.println("BLE HID toggled");
    }
  }

  // Thumbs Down Button: Single Click for haptics intensity tuning
  if(current_input_event->buttonStates.thumbsDownButton.event == ButtonEvent::SingleClick &&
     _lastInputEvent.buttonStates.thumbsDownButton.event != ButtonEvent::SingleClick) {
    if(!in_tuning) {
      tuning_target = 0; // Haptics
      in_tuning = true;
      Serial.println("Tuning Haptics Intensity - Use Thumb Up (+) / Down (-) to adjust");
    } else {
      sm.decreaseSetting(tuning_target, 10);
    }
  }

  // Thumbs Down Button: Double Click for laser intensity tuning
  if(current_input_event->buttonStates.thumbsDownButton.event == ButtonEvent::DoubleClick &&
     _lastInputEvent.buttonStates.thumbsDownButton.event != ButtonEvent::DoubleClick) {
    if(!in_tuning) {
      tuning_target = 1; // Laser
      in_tuning = true;
      Serial.println("Tuning Laser Intensity - Use Thumb Up (+) / Down (-) to adjust");
    }
  }

  // Thumbs Down Button: Triple Click for LED intensity tuning
  if(current_input_event->buttonStates.thumbsDownButton.event == ButtonEvent::TripleClick &&
     _lastInputEvent.buttonStates.thumbsDownButton.event != ButtonEvent::TripleClick) {
    if(!in_tuning) {
      tuning_target = 2; // LED
      in_tuning = true;
      Serial.println("Tuning LED Intensity - Use Thumb Up (+) / Down (-) to adjust");
    }
  }

  // Exit tuning mode on pointer button press
  if(current_input_event->buttonStates.pointerButton.isPressed && in_tuning) {
    in_tuning = false;
    sm.printCurrentSettings();
    Serial.println("Exited tuning mode");
  }

  // Haptics feedback on events
  if((current_input_event->buttonStates.pointerButton.event != _lastInputEvent.buttonStates.pointerButton.event ||
      current_input_event->buttonStates.thumbsUpButton.event != _lastInputEvent.buttonStates.thumbsUpButton.event ||
      current_input_event->buttonStates.thumbsDownButton.event != _lastInputEvent.buttonStates.thumbsDownButton.event) &&
     xEventGroupGetBits(device_mode_event_group) & USING_HAPTICS) {
    xQueueSend(haptics_queue, current_input_event, portMAX_DELAY);
  }
}

void ButtonTask::toggle_event_group_bit(EventBits_t bit) {
  if(xEventGroupGetBits(device_mode_event_group) & bit) {
    xEventGroupClearBits(device_mode_event_group, bit);
  } else {
    xEventGroupSetBits(device_mode_event_group, bit);
  }
}

void ButtonTask::send_queue(QueueHandle_t queue, EventBits_t bit) {
  if(current_input_event->buttonStates.pointerButton.isPressed  != _lastInputEvent.buttonStates.pointerButton.isPressed ||
     current_input_event->buttonStates.pointerButton.event      != _lastInputEvent.buttonStates.pointerButton.event ||
     current_input_event->buttonStates.thumbsDownButton.isPressed != _lastInputEvent.buttonStates.thumbsDownButton.isPressed ||
     current_input_event->buttonStates.thumbsUpButton.event     != _lastInputEvent.buttonStates.thumbsUpButton.event ||
     current_input_event->buttonStates.thumbsDownButton.isPressed != _lastInputEvent.buttonStates.thumbsDownButton.isPressed ||
     current_input_event->buttonStates.thumbsDownButton.event   != _lastInputEvent.buttonStates.thumbsDownButton.event) {
    // TODO: Check for why sent multiple times
    // Serial.println("Sending button event to output tasks...");
    if(xEventGroupGetBits(device_mode_event_group) & bit) {
      xQueueSend(queue, current_input_event, portMAX_DELAY);
    }
  }
}