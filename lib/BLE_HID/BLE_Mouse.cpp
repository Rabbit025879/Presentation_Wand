#include "BLE_Mouse.h"

// -- BLEMouse class --
void BLEMouse::buttons(uint8_t b) {
  if (b != _buttons) {
    _buttons = b;
    move(0,0,0,0);
  }
}

void BLEMouse::rawAction(uint8_t msg[], char msgSize) {}

void BLEMouse::click(uint8_t b) {
  _buttons = b;
  move(0,0,0,0);
  _buttons = 0;
  move(0,0,0,0);
}

void BLEMouse::move(signed char x, signed char y, signed char wheel, signed char hWheel) {
  if (this->isConnected()) {
    uint8_t m[5];
    m[0] = _buttons;
    m[1] = x;
    m[2] = y;
    m[3] = wheel;
    m[4] = hWheel;
    this->inputMouse->setValue(m, 5);
    this->inputMouse->notify();
#if defined(USE_NIMBLE)        
    vTaskDelay(pdMS_TO_TICKS(_delay_ms));
    // this->delay_ms(_delay_ms);
#endif // USE_NIMBLE
  }
}

void BLEMouse::press(uint8_t b) {
  buttons(_buttons | b);
}

void BLEMouse::release(uint8_t b) {
  buttons(_buttons & ~b);
}

bool BLEMouse::isPressed(uint8_t b) {
  if ((b & _buttons) > 0)
    return true;
  return false;
}