#include "main.h" 

// Global variables for FreeRTOS objects
static EventGroupHandle_t device_mode_event_group = NULL;
static QueueHandle_t haptics_queue = NULL;
static QueueHandle_t laser_queue = NULL;
static QueueHandle_t hid_queue = NULL;
static SystemMode* currentSystemMode = new SystemMode{FunctionMode::Presentation, InputMode::SimpleInput};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // mpu.begin();

  // Setup Tasks
  device_mode_event_group = xEventGroupCreate();
  xEventGroupSetBits(device_mode_event_group, USING_HAPTICS | USING_LASER | USING_HID | USING_MPU);

  haptics_queue = xQueueCreate(3, sizeof(ButtonState));
  laser_queue = xQueueCreate(3, sizeof(ButtonState));
  hid_queue = xQueueCreate(3, sizeof(ButtonState));

  ButtonTask::button_task_start(haptics_queue, laser_queue, hid_queue, device_mode_event_group, currentSystemMode, BUTTON_PIN); // Pin 10 for button
  HapticsTask::haptics_task_start(haptics_queue, device_mode_event_group, currentSystemMode, HAPTICS_PIN); // Pin A0 for haptics
  LaserTask::laser_task_start(laser_queue, device_mode_event_group, currentSystemMode, LASER_PIN); // Pin A1 for laser
  HIDTask::hid_task_start(hid_queue, device_mode_event_group, currentSystemMode);
  OTATask::ota_task_start(device_mode_event_group);

}

void loop() {
  // put your main code here, to run repeatedly:

  // if(Serial.available()) {
  //   String c = Serial.readString();
  //   if(c == "a") {
  //     Serial.print("accel: ");
  //     Serial.print(mpu.getAccel().acceleration.x);
  //     Serial.print(mpu.getAccel().acceleration.y);
  //     Serial.println(mpu.getAccel().acceleration.z);
  //   } else if(c == "g") {
  //     Serial.print("gyro: ");
  //     Serial.print(mpu.getGyro().gyro.x);
  //     Serial.print(mpu.getGyro().gyro.y);
  //     Serial.println(mpu.getGyro().gyro.z);
  //   } else if(c == "t") {
  //     Serial.print("temp: ");
  //     Serial.println(mpu.getTemp());
  //   }
  // }
}

// #include "Wire.h"
// #include <MPU6050_light.h>

// MPU6050 mpu(Wire);

// long timer = 0;

// void setup() {
//   delay(2000); // wait for Leonardo enumeration, others continue immediately
//   Serial.begin(9600);
//   Wire.begin();
  
//   byte status = mpu.begin();
//   Serial.print(F("MPU6050 status: "));
//   Serial.println(status);
//   while(status!=0){ } // stop everything if could not connect to MPU6050
  
//   Serial.println(F("Calculating offsets, do not move MPU6050"));
//   delay(1000);
//   mpu.calcOffsets(true,true); // gyro and accelero
//   Serial.println("Done!\n");
  
// }

// void loop() {
//   mpu.update();

//   if(millis() - timer > 1000){ // print data every second
//     Serial.print(F("TEMPERATURE: "));Serial.println(mpu.getTemp());
//     Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
//     Serial.print("\tY: ");Serial.print(mpu.getAccY());
//     Serial.print("\tZ: ");Serial.println(mpu.getAccZ());
  
//     Serial.print(F("GYRO      X: "));Serial.print(mpu.getGyroX());
//     Serial.print("\tY: ");Serial.print(mpu.getGyroY());
//     Serial.print("\tZ: ");Serial.println(mpu.getGyroZ());
  
//     Serial.print(F("ACC ANGLE X: "));Serial.print(mpu.getAccAngleX());
//     Serial.print("\tY: ");Serial.println(mpu.getAccAngleY());
    
//     Serial.print(F("ANGLE     X: "));Serial.print(mpu.getAngleX());
//     Serial.print("\tY: ");Serial.print(mpu.getAngleY());
//     Serial.print("\tZ: ");Serial.println(mpu.getAngleZ());
//     Serial.println(F("=====================================================\n"));
//     timer = millis();
//   }

// }