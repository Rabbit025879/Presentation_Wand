#ifndef BLE_MOUSE_H
#define BLE_MOUSE_H

#include <BleMouse.h>
#include <Arduino.h>

class BLE_Mouse : public BleMouse {
public:
  BLE_Mouse(std::string deviceName = "Tu's Wand(Mouse)", std::string deviceManufacturer = "Tu123", uint8_t batteryLevel = 100) 
    : BleMouse(deviceName, deviceManufacturer, batteryLevel) {}
  void begin();
  void moveTest();
};

#endif // BLE_MOUSE_H