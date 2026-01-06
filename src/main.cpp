#include "main.h"

// Adafruit_MPU6050 mpu;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  ota.begin();

  blehid.begin();

  // pinMode(23, OUTPUT);
  // mpu.begin();
  // mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

void loop() {
  // put your main code here, to run repeatedly:
  ota.loop(); // Handle OTA updates
  
  static uint8_t enterKey = 0;

  if(Serial.available()) {
    enterKey = Serial.parseInt();
    Serial.print("Enter Key: ");
    Serial.println(enterKey);
    if(blehid.isConnected() == false) {
      Serial.println("BLE Not Connected");
      return;
    }
    if(enterKey == 5) {
      Serial.println("Mouse Move Test");
      blehid.getMouse()->move(200, 0);
    } else if(enterKey) {
      Serial.println("Keyboard Key Test");
      blehid.getKeyboard()->write(enterKey);
    }
  }

  // sensors_event_t a, g, temp;
  // mpu.getEvent(&a, &g, &temp);

  // Serial.print("acce: ");
  // Serial.print(a.acceleration.x);
  // Serial.print(a.acceleration.y);
  // Serial.println(a.acceleration.z);

  // Serial.print("gyro: ");
  // Serial.print(g.gyro.x);
  // Serial.print(g.gyro.y);
  // Serial.println(g.gyro.z);
}