#ifndef MPU_H
#define MPU_H

#include "Utils.h"
#include <MPU6050_light.h>
// TODO: Immplement Motion Detection and Gesture Recognition
class MPU : public MPU6050 {
public:
  MPU(int gyro_config_num = 1, int acc_config_num = 0);
};

#endif // MPU_H