#ifndef MAIN_H
#define MAIN_H

#include "MPU.h"
#include "button_task.h"
#include "haptics_task.h"
#include "laser_task.h"
#include "hid_task.h"
#include "ota_task.h"

MPU mpu;
SystemConfig currentSystemConfig = {FunctionMode::Presentation, InputMode::SimpleInput};

#endif // MAIN_H