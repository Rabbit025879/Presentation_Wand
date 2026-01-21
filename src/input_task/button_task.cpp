#include "input_task/button_task.h"
#include "DeviceManager.h"
#include "Utils.h"

// Static instance for FreeRTOS callback
static ButtonTask* button_task_instance = nullptr;

ButtonTask::ButtonTask()
  : button_task_handle(NULL),
    haptics_queue(nullptr),
    laser_queue(nullptr),
    hid_queue(nullptr),
    device_manager(nullptr),
    current_input_event(nullptr),
    current_system_mode(nullptr),
    in_tuning(false),
    tuning_target(0) {}

void ButtonTask::start(
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

  for(;;) {
    // Update button states
    current_input_event->buttonStates = multi_button.getState();
    
    // Check for Command Mode entry (all three buttons held)
    if(multi_button.allThreeButtonsHold()) {
      if(current_system_mode->inputMode != InputMode::Command) {
        // Enter Command Mode
        current_system_mode->inputMode = InputMode::Command;
        DEBUG_PRINTLN("Entered Command Mode");
        xQueueSend(laser_queue, current_input_event, portMAX_DELAY);
        device_manager->printCurrentSettings();
      } else {
        // Restart device
        DEBUG_PRINTLN("Restarting Device...");
        current_input_event->buttonStates.thumbsDownButton.event = ButtonEvent::DummyEvent;
        xQueueSend(haptics_queue, current_input_event, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_restart();
      }
    }
    // Switch function mode (both thumb buttons held)
    if(multi_button.bothThumbButtonsHold() && current_system_mode->inputMode != InputMode::Command) {
      if(current_system_mode->functionMode == FunctionMode::Presentation) {
        current_system_mode->functionMode = FunctionMode::MediaControl;
        current_input_event->buttonStates.pointerButton.event = ButtonEvent::DummyEvent;
        xQueueSend(haptics_queue, current_input_event, portMAX_DELAY);
        DEBUG_PRINTLN("Switched to Media Control Mode");
      } else {  
        current_system_mode->functionMode = FunctionMode::Presentation;
        current_input_event->buttonStates.pointerButton.event = ButtonEvent::DummyEvent;
        xQueueSend(haptics_queue, current_input_event, portMAX_DELAY);
        DEBUG_PRINTLN("Switched to Presentation Mode");
      }
    }

    // Handle input modes
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
        handle_command_mode();
        break;

      default:
        break;
    }
    
    _lastInputEvent = *current_input_event;
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

// Motion control specific handling
void ButtonTask::motion_control_specific() { 
  // Triple click thumbs up: Toggle motion detect lock
  if(current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::TripleClick &&
     _lastInputEvent.buttonStates.thumbsUpButton.event != ButtonEvent::TripleClick) {
    device_manager->toggleMotionDetectLock();
    DEBUG_PRINT("Motion Detect Lock: ");
    DEBUG_PRINTLN(device_manager->isMotionDetectLocked() ? "LOCKED" : "UNLOCKED");
    return;
  }

  // Thumbs down button: Enable/disable motion detection
  if(current_input_event->buttonStates.thumbsDownButton.isPressed != 
     _lastInputEvent.buttonStates.thumbsDownButton.isPressed) {
    if(current_input_event->buttonStates.thumbsDownButton.isPressed) {
      if(!device_manager->isMotionDetectLocked()) {
        device_manager->enableFeature(USING_MPU);
        DEBUG_PRINTLN("Motion Detect: ON");
      }
    } else {
      if(!device_manager->isMotionDetectLocked()) {
        device_manager->disableFeature(USING_MPU);
        current_input_event->motionState = MotionState();
        DEBUG_PRINTLN("Motion Detect: OFF");
      }
    }
  }
}

void ButtonTask::handle_command_mode() {
  // Pointer: Single click to switch input mode
  if(current_input_event->buttonStates.pointerButton.event == ButtonEvent::SingleClick &&
     _lastInputEvent.buttonStates.pointerButton.event != ButtonEvent::SingleClick) {
    if(_lastInputMode == InputMode::SimpleInput) {
      current_system_mode->inputMode = InputMode::MotionControl;
      DEBUG_PRINTLN("Switched to Motion Control Mode");
    } else {
      current_system_mode->inputMode = InputMode::SimpleInput;
      DEBUG_PRINTLN("Switched to Simple Input Mode");
    }
  }

  // Pointer: Double click for haptics test
  if(current_input_event->buttonStates.pointerButton.event == ButtonEvent::DoubleClick &&
     _lastInputEvent.buttonStates.pointerButton.event != ButtonEvent::DoubleClick) {
    current_input_event->buttonStates.pointerButton.event = ButtonEvent::DummyEvent;
    send_queue(haptics_queue, USING_HAPTICS);
    DEBUG_PRINTLN("Haptics Test Triggered");
  }

  // Pointer: Triple click for LED test
  if(current_input_event->buttonStates.pointerButton.event == ButtonEvent::TripleClick &&
     _lastInputEvent.buttonStates.pointerButton.event != ButtonEvent::TripleClick) {
    current_input_event->buttonStates.pointerButton.event = ButtonEvent::DummyEvent;
    DEBUG_PRINTLN("LED Test Triggered");
  }

  // Thumbs Up: Single click to toggle haptics
  if(current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::SingleClick &&
     _lastInputEvent.buttonStates.thumbsUpButton.event != ButtonEvent::SingleClick) {
    if(in_tuning) {
      device_manager->increaseSetting(tuning_target, 10);
    } else {
      device_manager->toggleHaptics();
      DEBUG_PRINT("Haptics: ");
      DEBUG_PRINTLN(device_manager->isHapticsEnabled() ? "ON" : "OFF");
    }
  }

  // Thumbs Up: Double click to toggle laser
  if(current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::DoubleClick &&
     _lastInputEvent.buttonStates.thumbsUpButton.event != ButtonEvent::DoubleClick) {
    if(!in_tuning) {
      device_manager->toggleLaser();
      DEBUG_PRINT("Laser: ");
      DEBUG_PRINTLN(device_manager->isLaserEnabled() ? "ON" : "OFF");
    }
  }

  // Thumbs Up: Triple click to toggle BLE HID
  if(current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::TripleClick &&
     _lastInputEvent.buttonStates.thumbsUpButton.event != ButtonEvent::TripleClick) {
    if(!in_tuning) {
      device_manager->toggleHid();
      DEBUG_PRINT("BLE HID: ");
      DEBUG_PRINTLN(device_manager->isHidEnabled() ? "ON" : "OFF");
    }
  }

  // Thumbs Down: Single click for haptics intensity tuning
  if(current_input_event->buttonStates.thumbsDownButton.event == ButtonEvent::SingleClick &&
     _lastInputEvent.buttonStates.thumbsDownButton.event != ButtonEvent::SingleClick) {
    if(!in_tuning) {
      tuning_target = 0;
      in_tuning = true;
      DEBUG_PRINTLN("Tuning Haptics Intensity - Use Thumb Up (+) / Down (-) to adjust");
    } else {
      device_manager->decreaseSetting(tuning_target, 10);
    }
  }

  // Thumbs Down: Double click for laser intensity tuning
  if(current_input_event->buttonStates.thumbsDownButton.event == ButtonEvent::DoubleClick &&
     _lastInputEvent.buttonStates.thumbsDownButton.event != ButtonEvent::DoubleClick) {
    if(!in_tuning) {
      tuning_target = 1;
      in_tuning = true;
      DEBUG_PRINTLN("Tuning Laser Intensity - Use Thumb Up (+) / Down (-) to adjust");
    }
  }

  // Thumbs Down: Triple click for LED intensity tuning
  if(current_input_event->buttonStates.thumbsDownButton.event == ButtonEvent::TripleClick &&
     _lastInputEvent.buttonStates.thumbsDownButton.event != ButtonEvent::TripleClick) {
    if(!in_tuning) {
      tuning_target = 2;
      in_tuning = true;
      DEBUG_PRINTLN("Tuning LED Intensity - Use Thumb Up (+) / Down (-) to adjust");
    }
  }

  // Exit tuning mode on button hold
  if(in_tuning) {
    if(current_input_event->buttonStates.pointerButton.event == ButtonEvent::Hold ||
       current_input_event->buttonStates.thumbsUpButton.event == ButtonEvent::Hold ||
       current_input_event->buttonStates.thumbsDownButton.event == ButtonEvent::Hold) {
      in_tuning = false;
      device_manager->printCurrentSettings();
      DEBUG_PRINTLN("Exited tuning mode");
    }
  }

  // Haptics feedback on command mode events
  if((current_input_event->buttonStates.pointerButton.event != _lastInputEvent.buttonStates.pointerButton.event ||
      current_input_event->buttonStates.thumbsUpButton.event != _lastInputEvent.buttonStates.thumbsUpButton.event ||
      current_input_event->buttonStates.thumbsDownButton.event != _lastInputEvent.buttonStates.thumbsDownButton.event) &&
     device_manager->isHapticsEnabled()) {
    xQueueSend(haptics_queue, current_input_event, portMAX_DELAY);
  }
}

void ButtonTask::send_queue(QueueHandle_t queue, EventBits_t bit) {
  if(current_input_event->buttonStates.pointerButton.isPressed  != _lastInputEvent.buttonStates.pointerButton.isPressed ||
     current_input_event->buttonStates.pointerButton.event      != _lastInputEvent.buttonStates.pointerButton.event ||
     current_input_event->buttonStates.thumbsDownButton.isPressed != _lastInputEvent.buttonStates.thumbsDownButton.isPressed ||
     current_input_event->buttonStates.thumbsUpButton.event     != _lastInputEvent.buttonStates.thumbsUpButton.event ||
     current_input_event->buttonStates.thumbsDownButton.isPressed != _lastInputEvent.buttonStates.thumbsDownButton.isPressed ||
     current_input_event->buttonStates.thumbsDownButton.event   != _lastInputEvent.buttonStates.thumbsDownButton.event) {
    if(device_manager->isFeatureEnabled(bit)) {
      xQueueSend(queue, current_input_event, portMAX_DELAY);
    }
  }
}