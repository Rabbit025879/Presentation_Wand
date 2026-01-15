#ifndef MAIN_H
#define MAIN_H

#include "MPU.h"
#include "button_task.h"
#include "haptics_task.h"
#include "laser_task.h"
#include "hid_task.h"
#include "OTA.h"

#define USING_HAPTICS        (1 << 0)
#define USING_LASER          (1 << 1)
#define USING_HID            (1 << 2)
#define USING_MPU            (1 << 3)
#define USING_OTA_UPLOAD     (1 << 4)

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

OTA ota;
MPU mpu;
SystemConfig currentSystemConfig = {FunctionMode::Presentation, InputMode::SimpleInput};
#endif // MAIN_H