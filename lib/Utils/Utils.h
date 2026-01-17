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

#define BUTTON_PIN  10
#define HAPTICS_PIN A0
#define LASER_PIN   A1

#define BUTTON_DEBOUNCE_DEFAULT 50 // milliseconds
#define BUTTON_PRESS_WAIT_TIME 700 // milliseconds
#define BUTTON_SHORT_PRESS_THRESHOLD 200 // milliseconds

#define MOTION_DEBOUNCE_DEFAULT 500 // milliseconds
#define TILT_THRESHOLD 15.0 // degrees
#define FLICK_THRESHOLD 0.5 // g

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

// TODO: Modified to superposition of states if needed(maybe using bitmask)
enum class AttitudeState {
  None,
  // OutOfPlane
  TiltUp,
  TiltDown,
  TiltClockwise,
  TiltCounterClockwise,
  // InPlane
  TiltLeft,
  TiltRight
};

struct MotionState {
  MotionEvent motionEvent;
  AttitudeState attitudeState;

  MotionState() : motionEvent(MotionEvent::None), attitudeState(AttitudeState::None) {}
  MotionState(MotionEvent mevt, AttitudeState astate) : motionEvent(mevt), attitudeState(astate) {}
};

struct InputEvent {
  ButtonState buttonState;
  MotionState motionState;

  InputEvent() : buttonState(ButtonState()), motionState(MotionState()) {}
  InputEvent(ButtonState bstate, MotionState mstate) : buttonState(bstate), motionState(mstate) {}
};

enum class InputMode {
  SimpleInput,
  Command,
  MotionControl
};

enum class FunctionMode {
  Presentation,
  MediaControl,
  Mouse,
  Keyboard
};

struct SystemMode {
  FunctionMode mode;
  InputMode inputMode;

  SystemMode() : mode(FunctionMode::Presentation), inputMode(InputMode::SimpleInput) {}
  SystemMode(FunctionMode m, InputMode im) : mode(m), inputMode(im) {}
};

#endif // UTILS_H