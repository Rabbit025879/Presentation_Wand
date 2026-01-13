#ifndef BLE_HID_UTILS_H
#define BLE_HID_UTILS_H

#if defined(USE_NIMBLE)

#include "NimBLECharacteristic.h"
#include "NimBLEHIDDevice.h"

#define BLEDevice                  NimBLEDevice
#define BLEServerCallbacks         NimBLEServerCallbacks
#define BLECharacteristicCallbacks NimBLECharacteristicCallbacks
#define BLEHIDDevice               NimBLEHIDDevice
#define BLECharacteristic          NimBLECharacteristic
#define BLEAdvertising             NimBLEAdvertising
#define BLEServer                  NimBLEServer

#else

#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"

#endif // USE_NIMBLE

// BLE HID Utils Class
class BLEHIDUtils {
public:
  uint32_t  _delay_ms = 7;
  void delay_ms(uint64_t ms);
  void setDelay(uint32_t ms) {
    this->_delay_ms = ms;
  }
  void bindConnectionState(const bool* state) {
    _connected = state;
  }
  bool isConnected() {
    return _connected ? *_connected : false;
  }

private:
  const bool* _connected;
};

#endif // BLE_HID_UTILS_H