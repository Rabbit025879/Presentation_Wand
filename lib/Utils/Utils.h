#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <Preferences.h>

// Debug logging macro
#ifdef DEBUG_MODE
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(...)
#endif

// Feature flags (event bits)
#define USING_HAPTICS        (1 << 0)
#define USING_LASER          (1 << 1)
#define USING_HID            (1 << 2)
#define USING_MPU            (1 << 3)
#define USING_WEB_SERVER     (1 << 4)
#define USING_OTA_UPLOAD     (1 << 5)

// Task priorities
#define BUTTON_TASK_PRIORITY    4
#define HAPTICS_TASK_PRIORITY   2
#define LASER_TASK_PRIORITY     1
#define HID_TASK_PRIORITY       5
#define MPU_TASK_PRIORITY       3
#define OTA_TASK_PRIORITY       5

// Hardware pin definitions
#define POINTER_BUTTON_PIN     10
#define THUMBS_UP_BUTTON_PIN    7
#define THUMBS_DOWN_BUTTON_PIN  6
#define HAPTICS_PIN            A0
#define LASER_PIN              A1
#define LED_PIN                A2

// Timing constants (milliseconds)
#define BUTTON_DEBOUNCE_DEFAULT       50
#define BUTTON_PRESS_WAIT_TIME       200
#define BUTTON_SHORT_PRESS_THRESHOLD 150
#define MOTION_DEBOUNCE_DEFAULT      200
#define CONTINUOUS_EVENT_THROTTLE    500

// Motion detection thresholds
#define TILT_THRESHOLD   30.0  // degrees
#define FLICK_THRESHOLD  250.0 // degrees/second
#define ROTATE_THRESHOLD 700.0 // degrees/second

// Button event types
enum class ButtonEvent {
  None,
  SingleClick,
  DoubleClick,
  TripleClick,
  SingleLongPress,
  DoubleLongPress,
  TripleLongPress,
  Hold,
  DummyEvent, // For triggering actions without button press
  OtherPattern
};

// Button state
struct ButtonState {
  bool isPressed;
  ButtonEvent event;

  ButtonState() : isPressed(false), event(ButtonEvent::None) {}
  ButtonState(bool pressed, ButtonEvent evt) : isPressed(pressed), event(evt) {}
};

// Multi-button state (3 buttons: pointer, thumbs up/down)
struct MultiButtonState {
  ButtonState pointerButton;
  ButtonState thumbsUpButton;
  ButtonState thumbsDownButton;

  MultiButtonState() 
    : pointerButton(), thumbsUpButton(), thumbsDownButton() {}
  MultiButtonState(ButtonState pbutton, ButtonState ubbutton, ButtonState dbbutton) 
    : pointerButton(pbutton), thumbsUpButton(ubbutton), thumbsDownButton(dbbutton) {}
};

// Motion event types
enum class MotionEvent {
  None,
  FlickUp,               // Out-of-plane
  FlickDown,             // Out-of-plane
  RotateClockwise,       // Out-of-plane
  RotateCounterClockwise,// Out-of-plane
  FlickLeft,             // In-plane
  FlickRight             // In-plane
};

// Attitude state flags
#define ATTITUDE_STATE_NONE                  (0)
#define ATTITUDE_STATE_TILT_UP               (1 << 0)
#define ATTITUDE_STATE_TILT_DOWN             (1 << 1)
#define ATTITUDE_STATE_TILT_CLOCKWISE        (1 << 2)
#define ATTITUDE_STATE_TILT_COUNTERCLOCKWISE (1 << 3)
#define ATTITUDE_STATE_TILT_LEFT             (1 << 4) // In-plane (unreliable)
#define ATTITUDE_STATE_TILT_RIGHT            (1 << 5) // In-plane (unreliable)

// Motion state
struct MotionState {
  MotionEvent motionEvent;
  byte attitudeState;

  MotionState() : motionEvent(MotionEvent::None), attitudeState(ATTITUDE_STATE_NONE) {}
  MotionState(MotionEvent mevt, byte astate) : motionEvent(mevt), attitudeState(astate) {}
};

// Combined input event (buttons + motion)
struct InputEvent {
  MultiButtonState buttonStates;
  MotionState motionState;

  InputEvent() : buttonStates(), motionState() {}
  InputEvent(ButtonState pbutton, ButtonState ubbutton, ButtonState dbbutton, MotionState mstate) 
    : buttonStates(pbutton, ubbutton, dbbutton), motionState(mstate) {}
};

// Input modes
enum class InputMode {
  SimpleInput,   // Basic button input only
  MotionControl, // Button + motion input
  Command        // Configuration mode
};

// Function modes
enum class FunctionMode {
  Presentation,  // Presentation control
  MediaControl,  // Media playback control
  Mouse,         // Mouse emulation (future)
  Keyboard       // Keyboard emulation (future)
};

// System mode (combines function + input mode)
struct SystemMode {
  FunctionMode functionMode;
  InputMode inputMode;

  SystemMode() : functionMode(FunctionMode::Presentation), inputMode(InputMode::SimpleInput) {}
  SystemMode(FunctionMode fm, InputMode im) : functionMode(fm), inputMode(im) {}
};

#endif // UTILS_H