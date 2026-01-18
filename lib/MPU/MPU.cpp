#include "MPU.h"

MPU::MPU(int gyro_config_num, int acc_config_num) 
  : MPU6050(Wire), _gyro_config(gyro_config_num), _acc_config(acc_config_num) {}

bool MPU::begin() {
  Wire.begin();
  
  if(MPU6050::begin(_gyro_config, _acc_config) != 0) {
    Serial.println(F("MPU6050 connection failed. Please check your connection with MPU6050."));
    return false;
  }
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  MPU6050::calcOffsets(true, true);
  Serial.println("Done!\n");
  return true;
}

Vec3 MPU::_normalize(Vec3 v) {
  float mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  if (mag < 0.0001f) return Vec3(0, 0, 1); // Return default if zero vector
  return Vec3(v.x / mag, v.y / mag, v.z / mag);
}

Vec3 MPU::_cross(Vec3 a, Vec3 b) {
  return Vec3(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  );
}

float MPU::_dot(Vec3 a, Vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 MPU::_transformToDeviceFrame(Vec3 gyro_global, Vec3 gravity) {
  // Normalize gravity vector to get device Z-axis in global frame
  Vec3 z_device = _normalize(gravity);
  
  // Choose an arbitrary reference vector (global X-axis preferred)
  Vec3 reference(1, 0, 0);
  
  // If gravity is too aligned with reference, use different reference
  if (fabs(_dot(z_device, reference)) > 0.99f) {
    reference = Vec3(0, 1, 0);
  }
  
  // Calculate device X-axis (perpendicular to gravity)
  Vec3 x_device = _normalize(_cross(reference, z_device));
  
  // Calculate device Y-axis (completes right-handed coordinate system)
  Vec3 y_device = _cross(z_device, x_device);
  
  // Transform gyro vector from global to device frame using rotation matrix
  // gyro_device = R^T * gyro_global, where R = [x_device, y_device, z_device]
  Vec3 gyro_device(
    _dot(x_device, gyro_global),
    _dot(y_device, gyro_global),
    _dot(z_device, gyro_global)
  );
  
  return gyro_device;
}

Vec3 MPU::_getGyroDeviceFrame() {
  // Get raw sensor data in global frame
  Vec3 gyro_global(getGyroX(), getGyroY(), getGyroZ());
  Vec3 accel(getAccX(), getAccY(), getAccZ());
  
  // Transform to device frame where Z aligns with gravity
  return _transformToDeviceFrame(gyro_global, accel);
}

MotionEvent MPU::_detectEvent() {
  if(millis() - _eventLastUpdateTime < MOTION_DEBOUNCE_DEFAULT) {
    return _event; // Debounce
  }
  _eventLastUpdateTime = millis();

  // Get gyro data in device frame (gravity-aligned)
  Vec3 gyro_device = _getGyroDeviceFrame();
  // Now gyro_device.x, gyro_device.y, gyro_device.z are in device frame
  // where Z-axis aligns with gravity direction

  // Detect rotation events (around gravity axis - Z)
  if (gyro_device.y > FLICK_THRESHOLD) {
    _event = MotionEvent::RotateCounterClockwise;
  } else if (gyro_device.y < -FLICK_THRESHOLD) {
    _event = MotionEvent::RotateClockwise;
  }
  // Detect flick events (perpendicular to gravity)
  else if (gyro_device.x > FLICK_THRESHOLD) {
    _event = MotionEvent::FlickDown;
  } else if (gyro_device.x < -FLICK_THRESHOLD) {
    _event = MotionEvent::FlickUp;
  } else if (gyro_device.z > FLICK_THRESHOLD) {
    _event = MotionEvent::FlickLeft;
  } else if (gyro_device.z < -FLICK_THRESHOLD) {
    _event = MotionEvent::FlickRight;
  } else {
    _event = MotionEvent::None;
  }

  return _event;
}

byte MPU::_detectAttitude() {
  if(millis() - _attitudeLastUpdateTime < MOTION_DEBOUNCE_DEFAULT) {
    return _attitude; // Debounce
  }
  _attitudeLastUpdateTime = millis();
  
  float angleX = getAngleX();
  float angleY = getAngleY();

  _attitude = ATTITUDE_STATE_NONE;

  if(angleX > TILT_THRESHOLD) {
    _attitude |= ATTITUDE_STATE_TILT_DOWN;
  } else if(angleX < -TILT_THRESHOLD) {
    _attitude |= ATTITUDE_STATE_TILT_UP;
  }

  if(angleY > TILT_THRESHOLD) {
    _attitude |= ATTITUDE_STATE_TILT_COUNTERCLOCKWISE;
  } else if(angleY < -TILT_THRESHOLD) {
    _attitude |= ATTITUDE_STATE_TILT_CLOCKWISE;
  }

  return _attitude;
}