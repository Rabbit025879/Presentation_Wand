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

MotionEvent MPU::_detectEvent() {
  Vec3 gyro{getGyroX(), getGyroY(), getGyroZ()};
  if(millis() - _eventLastUpdateTime < MOTION_DEBOUNCE_DEFAULT) {
    return _event; // Debounce
  }

  if (gyro.y > ROTATE_THRESHOLD) {
    if(_event == MotionEvent::RotateClockwise) {
      // To avoid stop continous event
      _eventLastUpdateTime = millis();
      _event = MotionEvent::None;
      return _event;
    }
    _event = MotionEvent::RotateCounterClockwise;
    _eventLastUpdateTime = millis();
  } else if (gyro.y < -ROTATE_THRESHOLD) {
    if(_event == MotionEvent::RotateCounterClockwise) {
      // To avoid stop continous event
      _eventLastUpdateTime = millis();
      _event = MotionEvent::None;
      return _event;
    }
    _event = MotionEvent::RotateClockwise;
    _eventLastUpdateTime = millis();
  } else if (gyro.x > FLICK_THRESHOLD) {
    _event = MotionEvent::FlickDown;
    _eventLastUpdateTime = millis();
  } else if (gyro.x < -FLICK_THRESHOLD) {
    _event = MotionEvent::FlickUp;
    _eventLastUpdateTime = millis();
  } else if (gyro.z > FLICK_THRESHOLD) {
    _event = MotionEvent::FlickLeft;
    _eventLastUpdateTime = millis();
  } else if (gyro.z < -FLICK_THRESHOLD) {
    _event = MotionEvent::FlickRight;
    _eventLastUpdateTime = millis();
  } else {
    if(_event != MotionEvent::RotateClockwise &&
       _event != MotionEvent::RotateCounterClockwise) {
      _event = MotionEvent::None;
    }
  }

  return _event;
}

byte MPU::_detectAttitude() {
  if(millis() - _attitudeLastUpdateTime < MOTION_DEBOUNCE_DEFAULT) {
    return _attitude; // Debounce
  }
  _attitudeLastUpdateTime = millis();
  
  Vec3 angle{getAngleX(), getAngleY(), getAngleZ()};

  _attitude = ATTITUDE_STATE_NONE;

  if(angle.x > TILT_THRESHOLD) {
    _attitude |= ATTITUDE_STATE_TILT_DOWN;
  } else if(angle.x < -TILT_THRESHOLD) {
    _attitude |= ATTITUDE_STATE_TILT_UP;
  }

  if(angle.y > TILT_THRESHOLD) {
    _attitude |= ATTITUDE_STATE_TILT_COUNTERCLOCKWISE;
  } else if(angle.y < -TILT_THRESHOLD) {
    _attitude |= ATTITUDE_STATE_TILT_CLOCKWISE;
  }

  return _attitude;
}