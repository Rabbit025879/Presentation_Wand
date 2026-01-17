#include "MPU.h"

MPU::MPU(int gyro_config_num, int acc_config_num) 
  : MPU6050(Wire), gyro_config(gyro_config_num), acc_config(acc_config_num) {}

bool MPU::begin() {
  Wire.begin();
  
  if(MPU6050::begin() != 0) {
    Serial.println(F("MPU6050 connection failed. Please check your connection with MPU6050."));
    return false;
  }
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  MPU6050::calcOffsets(true, true);
  Serial.println("Done!\n");
  return true;
}

