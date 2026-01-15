#ifndef UTILS_H
#define UTILS_H

#define USING_HAPTICS        (1 << 0)
#define USING_LASER          (1 << 1)
#define USING_HID            (1 << 2)
#define USING_MPU            (1 << 3)
#define USING_WEB_SERVER     (1 << 4)
#define USING_OTA_UPLOAD     (1 << 5)

#define BUTTON_PIN  10
#define HAPTICS_PIN A0
#define LASER_PIN   A1

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

struct SystemConfig {
  FunctionMode mode;
  InputMode inputMode;
};

#endif // UTILS_H