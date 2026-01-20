#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include "Utils.h"
// TODO: To be implemented: Persistent storage (e.g., EEPROM, SPIFFS)
class SettingsManager {
public:
  SettingsManager();
  
  void increaseSetting(uint8_t setting_id, uint8_t step = 10);
  void decreaseSetting(uint8_t setting_id, uint8_t step = 10);
  void printCurrentSettings();
  
  uint8_t getHapticsIntensity() const { return device_settings.hapticsIntensity; }
  uint8_t getLaserIntensity() const { return device_settings.laserIntensity; }
  uint8_t getLedIntensity() const { return device_settings.ledIntensity; }
  
  void setHapticsIntensity(uint8_t value) { device_settings.hapticsIntensity = (value > 255) ? 255 : value; }
  void setLaserIntensity(uint8_t value) { device_settings.laserIntensity = (value > 255) ? 255 : value; }
  void setLedIntensity(uint8_t value) { device_settings.ledIntensity = (value > 255) ? 255 : value; }
  
  bool isHapticsEnabled() const { return device_settings.hapticsEnabled; }
  bool isLaserEnabled() const { return device_settings.laserEnabled; }
  bool isLedEnabled() const { return device_settings.ledEnabled; }
  
  void toggleHaptics() { device_settings.hapticsEnabled = !device_settings.hapticsEnabled; }
  void toggleLaser() { device_settings.laserEnabled = !device_settings.laserEnabled; }
  void toggleLed() { device_settings.ledEnabled = !device_settings.ledEnabled; }
  
  bool isMotionDetectLocked() const { return device_settings.motionDetectLocked; }
  void toggleMotionDetectLock() { device_settings.motionDetectLocked = !device_settings.motionDetectLocked; }
  
private:
  DeviceSettings device_settings;
  uint8_t current_tuning_target; // 0=haptics, 1=laser, 2=led
};

#endif // SETTINGS_MANAGER_H
