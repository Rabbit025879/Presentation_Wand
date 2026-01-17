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
  MPU(int gyro_config_num = 1, int acc_config_num = 0);
  bool begin();  // Returns success/failure
  MotionState getMotionState() {
    return MotionState(_detectEvent(), _detectAttitude());
  }
  
  
private:
  int _gyro_config;
  int _acc_config;
  MotionEvent _event = MotionEvent::None;
  AttitudeState _attitude = AttitudeState::None;
  uint32_t _eventLastUpdateTime = 0;
  uint32_t _attitudeLastUpdateTime = 0;
  
  MotionEvent _detectEvent();
  AttitudeState _detectAttitude();
  
  // Get gyro data in device frame (gravity-aligned)
  Vec3 _getGyroDeviceFrame();
  // Transform gyro from global frame to device frame
  Vec3 _transformToDeviceFrame(Vec3 gyro_global, Vec3 gravity);
  // Normalize vector
  Vec3 _normalize(Vec3 v);
  // Cross product
  Vec3 _cross(Vec3 a, Vec3 b);
  // Dot product
  float _dot(Vec3 a, Vec3 b);
};

#endif // MPU_H