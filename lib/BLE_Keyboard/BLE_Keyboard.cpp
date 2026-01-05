#include "BLE_Keyboard.h"

void BLE_Keyboard::begin() {
  BleKeyboard::begin();
  while(!this->isConnected()) {
    Serial.println("BLE Keyboard Connecting...");
    delay(300);
  }
  
  Serial.println("BLE Keyboard Connected !!");
}

void BLE_Keyboard::sendKey(uint8_t key) {
  if(this->isConnected()) {
    this->write(key);
    Serial.println(key);
  } else {
    Serial.println("BLE Keyboard Disconnected !!");
    while(!this->isConnected()) {
      Serial.println("BLE Keyboard Reconnecting...");
      delay(300);
    } 
  }
}