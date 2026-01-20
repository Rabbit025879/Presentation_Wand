#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

#define USING_HAPTICS        (1 << 0)
#define USING_LASER          (1 << 1)
#define USING_HID            (1 << 2)
#define USING_MPU            (1 << 3)
#define USING_WEB_SERVER     (1 << 4)
#define USING_OTA_UPLOAD     (1 << 5)

#define BUTTON_TASK_PRIORITY    4
#define HAPTICS_TASK_PRIORITY   2
#define LASER_TASK_PRIORITY     1
#define HID_TASK_PRIORITY       5
#define MPU_TASK_PRIORITY       3
#define OTA_TASK_PRIORITY       5

#define POINTER_BUTTON_PIN     10
#define THUMBS_UP_BUTTON_PIN    7
#define THUMBS_DOWN_BUTTON_PIN  6
#define HAPTICS_PIN A0
#define LASER_PIN   A1
#define LED_PIN     A2

#define BUTTON_DEBOUNCE_DEFAULT 50 // milliseconds
#define BUTTON_PRESS_WAIT_TIME 200 // milliseconds
#define BUTTON_SHORT_PRESS_THRESHOLD 150 // milliseconds

#define MOTION_DEBOUNCE_DEFAULT 200 // milliseconds
#define TILT_THRESHOLD 30.0 // degrees
#define FLICK_THRESHOLD 250.0 // degrees/second
#define ROTATE_THRESHOLD 700.0 // degrees/second
#define CONTINUOUS_EVENT_THROTTLE 500 // milliseconds

enum class ButtonEvent {
  None,
  SingleClick,
  DoubleClick,
  TripleClick,
  SingleLongPress,
  DoubleLongPress,
  TripleLongPress,
  Hold,
  OtherPattern
};

struct ButtonState {
  bool isPressed;
  ButtonEvent event;

  ButtonState() : isPressed(false), event(ButtonEvent::None) {}
  ButtonState(bool pressed, ButtonEvent evt) : isPressed(pressed), event(evt) {}
};

struct MultiButtonState {
  ButtonState pointerButton;
  ButtonState thumbsUpButton;
  ButtonState thumbsDownButton;

  MultiButtonState() 
    : pointerButton(ButtonState()), 
      thumbsUpButton(ButtonState()), 
      thumbsDownButton(ButtonState()) {}
  MultiButtonState(ButtonState pbutton, ButtonState ubbutton, ButtonState dbbutton) 
    : pointerButton(pbutton), 
      thumbsUpButton(ubbutton), 
      thumbsDownButton(dbbutton) {}
};

enum class MotionEvent {
  None,
  // OutOfPlane
  FlickUp,
  FlickDown,
  RotateClockwise,
  RotateCounterClockwise,
  // InPlane
  FlickLeft,
  FlickRight
};

// AttitudeState bitmask
// OutOfPlane
#define ATTITUDE_STATE_NONE                  (0)
#define ATTITUDE_STATE_TILT_UP               (1 << 0)
#define ATTITUDE_STATE_TILT_DOWN             (1 << 1)
#define ATTITUDE_STATE_TILT_CLOCKWISE        (1 << 2)
#define ATTITUDE_STATE_TILT_COUNTERCLOCKWISE (1 << 3)
// InPlane(Can't detect reliably)
#define ATTITUDE_STATE_TILT_LEFT             (1 << 4)
#define ATTITUDE_STATE_TILT_RIGHT            (1 << 5)

struct MotionState {
  MotionEvent motionEvent;
  byte attitudeState;

  MotionState() : motionEvent(MotionEvent::None), attitudeState(ATTITUDE_STATE_NONE) {}
  MotionState(MotionEvent mevt, byte astate) : motionEvent(mevt), attitudeState(astate) {}
};

// Multi-button support: Pointer, ThumbsUp, ThumbsDown
struct InputEvent {
  MultiButtonState buttonStates;
  MotionState motionState;

  InputEvent() 
      : buttonStates(MultiButtonState()), 
      motionState(MotionState()) {}
  InputEvent(ButtonState pbutton, ButtonState ubbutton, ButtonState dbbutton, MotionState mstate) 
    : buttonStates(MultiButtonState(pbutton, ubbutton, dbbutton)), 
      motionState(mstate) {}
};

enum class InputMode {
  SimpleInput,
  MotionControl,
  Command
};

enum class FunctionMode {
  Presentation,
  MediaControl,
  Mouse,
  Keyboard
};

enum class ButtonID {
  Pointer,
  ThumbsUp,
  ThumbsDown
};

struct SystemMode {
  FunctionMode functionMode;
  InputMode inputMode;

  SystemMode() : functionMode(FunctionMode::Presentation), inputMode(InputMode::SimpleInput) {}
  SystemMode(FunctionMode fm, InputMode im) : functionMode(fm), inputMode(im) {}
};

// Settings and status tracking structure
struct DeviceSettings {
  uint8_t hapticsIntensity;
  uint8_t laserIntensity;
  uint8_t ledIntensity;
  bool hapticsEnabled;
  bool laserEnabled;
  bool ledEnabled;
  bool motionDetectLocked;
  
  DeviceSettings() 
    : hapticsIntensity(100), 
      laserIntensity(100), 
      ledIntensity(100),
      hapticsEnabled(true), 
      laserEnabled(true), 
      ledEnabled(true),
      motionDetectLocked(false) {}
};

#endif // UTILS_H