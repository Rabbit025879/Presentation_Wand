#include "SettingsManager.h"

SettingsManager::SettingsManager()
  : device_settings(), current_tuning_target(0) {
}

void SettingsManager::increaseSetting(uint8_t setting_id, uint8_t step) {
  uint8_t new_value;
  switch(setting_id) {
    case 0: // Haptics
      new_value = device_settings.hapticsIntensity + step;
      device_settings.hapticsIntensity = (new_value > 255) ? 255 : new_value;
      Serial.print("Haptics Intensity: ");
      Serial.println(device_settings.hapticsIntensity);
      break;
    case 1: // Laser
      new_value = device_settings.laserIntensity + step;
      device_settings.laserIntensity = (new_value > 255) ? 255 : new_value;
      Serial.print("Laser Intensity: ");
      Serial.println(device_settings.laserIntensity);
      break;
    case 2: // LED
      new_value = device_settings.ledIntensity + step;
      device_settings.ledIntensity = (new_value > 255) ? 255 : new_value;
      Serial.print("LED Intensity: ");
      Serial.println(device_settings.ledIntensity);
      break;
    default:
      break;
  }
}

void SettingsManager::decreaseSetting(uint8_t setting_id, uint8_t step) {
  int16_t new_value;
  switch(setting_id) {
    case 0: // Haptics
      new_value = device_settings.hapticsIntensity - step;
      device_settings.hapticsIntensity = (new_value < 0) ? 0 : new_value;
      Serial.print("Haptics Intensity: ");
      Serial.println(device_settings.hapticsIntensity);
      break;
    case 1: // Laser
      new_value = device_settings.laserIntensity - step;
      device_settings.laserIntensity = (new_value < 0) ? 0 : new_value;
      Serial.print("Laser Intensity: ");
      Serial.println(device_settings.laserIntensity);
      break;
    case 2: // LED
      new_value = device_settings.ledIntensity - step;
      device_settings.ledIntensity = (new_value < 0) ? 0 : new_value;
      Serial.print("LED Intensity: ");
      Serial.println(device_settings.ledIntensity);
      break;
    default:
      break;
  }
}

void SettingsManager::printCurrentSettings() {
  Serial.println("\n========== Device Settings ==========");
  Serial.print("Haptics: ");
  Serial.print(device_settings.hapticsEnabled ? "ON" : "OFF");
  Serial.print(" | Intensity: ");
  Serial.println(device_settings.hapticsIntensity);
  
  Serial.print("Laser: ");
  Serial.print(device_settings.laserEnabled ? "ON" : "OFF");
  Serial.print(" | Intensity: ");
  Serial.println(device_settings.laserIntensity);
  
  Serial.print("LED: ");
  Serial.print(device_settings.ledEnabled ? "ON" : "OFF");
  Serial.print(" | Intensity: ");
  Serial.println(device_settings.ledIntensity);
  
  Serial.print("Motion Detect Locked: ");
  Serial.println(device_settings.motionDetectLocked ? "YES" : "NO");
  Serial.println("=====================================\n");
}
