#include "BLE_Mouse.h"

void BLE_Mouse::begin() {
  BleMouse::begin();

  while(!this->isConnected()) {
    Serial.println("BLE Mouse Connecting...");
    delay(300);
  }
  
  Serial.println("BLE Mouse Connected !!");
}

void BLE_Mouse::moveTest() {
  if(this->isConnected()) {
    this->move(50, 0); // Move mouse cursor 5 units to the right
  } else {
    Serial.println("BLE Mouse Disconnected !!");
    while(!this->isConnected()) {
      Serial.println("BLE Mouse Reconnecting...");
      delay(300);
    } 
  }
}