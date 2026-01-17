#include "MPU.h"

MPU::MPU(int gyro_config_num, int acc_config_num) : MPU6050(Wire) {
  Wire.begin();

  while(MPU6050::begin() != 0){
    delay(500);
    Serial.println(F("MPU6050 connection failed. Please check your connection with MPU6050."));
  } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  MPU6050::calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");
}

