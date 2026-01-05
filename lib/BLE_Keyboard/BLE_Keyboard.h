#ifndef BLE_KEYBOARD_H
#define BLE_KEYBOARD_H

#include <BleKeyboard.h>
#include <Arduino.h>


class BLE_Keyboard : public BleKeyboard {
public:
  BLE_Keyboard(std::string deviceName = "Tu's Wand", std::string deviceManufacturer = "Tu123", uint8_t batteryLevel = 100) 
    : BleKeyboard(deviceName, deviceManufacturer, batteryLevel) {}
  void begin();
  void sendKey(uint8_t key);
};

#endif // BLE_KEYBOARD_H