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
  Shake,
  TiltUp,
  TiltDown,
  TiltLeft,
  TiltRight,
  RotateClockwise,
  RotateCounterClockwise
};

struct InputEvent {
  ButtonState buttonState;
  MotionEvent motionEvent;

  InputEvent() : buttonState(ButtonState()), motionEvent(MotionEvent::None) {}
  InputEvent(ButtonState bstate, MotionEvent mevt) : buttonState(bstate), motionEvent(mevt) {}
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
};

#endif // UTILS_H