// This library features BLE HID combo for keyboard & mouse adapted from the ESP32-BLE-Keyboard & ESP32-BLE-Mouse library with NimBLE support

#ifndef BLE_COMBO_H
#define BLE_COMBO_H

#include "sdkconfig.h"

#if defined(CONFIG_BT_ENABLED)

#include "Print.h"

#include "Utils.h"
#include "BLE_Mouse.h"
#include "BLE_Keyboard.h"

// BLE HID Combo Class
class BLEHID : public BLEServerCallbacks, public BLECharacteristicCallbacks
{
private:
  BLEKeyboard     keyboard;
  BLEMouse        mouse;

  BLEHIDDevice*      hid;
  BLEAdvertising*    advertising;
  
  std::string        deviceName;
  std::string        deviceManufacturer;
  uint8_t            batteryLevel;
  bool               connected = false;

  uint16_t vid       = 0x05ac;
  uint16_t pid       = 0x820a;
  uint16_t version   = 0x0210;

public:
  BLEHID(std::string deviceName = "ESP32 BLE HID", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
  void begin(void);
  void end(void);

  // BLE setup functions
  bool isConnected(void);
  void setBatteryLevel(uint8_t level);
  void setName(std::string deviceName);  

  void set_vendor_id(uint16_t vid);
  void set_product_id(uint16_t pid);
  void set_version(uint16_t version);

  // Get Mouse functions
  BLEMouse& getMouse() {
    return this->mouse;
  }

  // Get Keyboard functions
  BLEKeyboard& getKeyboard() {
    return this->keyboard;
  }

protected:
  virtual void onStarted(BLEServer *pServer) { };
  virtual void onConnect(BLEServer* pServer) override;
  virtual void onDisconnect(BLEServer* pServer) override;
  virtual void onWrite(BLECharacteristic* me) override;
};

#endif // CONFIG_BT_ENABLED
#endif // BLE_COMBO_H
