#ifndef MPU_H
#define MPU_H

#include "Utils.h"
#include <MPU6050_light.h>

struct Vec3 {
  float x, y, z;
  Vec3(float x_ = 0, float y_ = 0, float z_ = 0) : x(x_), y(y_), z(z_) {}
};

class MPU : public MPU6050 {
public:
  MPU(int gyro_config_num = 2, int acc_config_num = 0);
  bool begin();  // Returns success/failure
  MotionState getMotionState() {
    return MotionState(_detectEvent(), _detectAttitude());
  }
  void resetEventDetection() {
    _event = MotionEvent::None;
  }
  void resetAttitudeDetection() {
    _attitude = ATTITUDE_STATE_NONE;
  }

private:
  int _gyro_config;
  int _acc_config;
  MotionEvent _event = MotionEvent::None;
  byte _attitude = ATTITUDE_STATE_NONE;
  uint32_t _eventLastUpdateTime = 0;
  uint32_t _attitudeLastUpdateTime = 0;
  
  MotionEvent _detectEvent();
  byte _detectAttitude();
};

#endif // MPU_H