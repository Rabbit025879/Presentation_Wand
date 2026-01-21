#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "Utils.h"

// Settings and status tracking structure
struct DeviceSettings {
  // Intensity levels: 0-100%
  uint8_t hapticsIntensity;
  uint8_t laserIntensity;
  uint8_t ledIntensity;
  
  DeviceSettings() 
    : hapticsIntensity(50), 
      laserIntensity(50), 
      ledIntensity(50) {}
};

struct WifiSettings {
  String mode;
  String ssid;
  String password;

  WifiSettings() : mode("AP"), ssid("Wand"), password("00000000") {}
};

class DeviceManager {
public:
  DeviceManager();
  
  void begin();

  void increaseSetting(uint8_t setting_id, uint8_t step = 10);
  void decreaseSetting(uint8_t setting_id, uint8_t step = 10);
  void printCurrentSettings();
  
  uint8_t getHapticsIntensity() const { return device_settings.hapticsIntensity*255/100; }
  uint8_t getLaserIntensity() const { return device_settings.laserIntensity*255/100; }
  uint8_t getLedIntensity() const { return device_settings.ledIntensity*255/100; }

  // Feature management (event group operations)
  void enableFeature(EventBits_t feature_bit);
  void disableFeature(EventBits_t feature_bit);
  void toggleFeature(EventBits_t feature_bit);
  bool isFeatureEnabled(EventBits_t feature_bit) const;
  EventBits_t getEnabledFeatures() const;

  // Convenience feature toggles
  void toggleHaptics() { toggleFeature(USING_HAPTICS); }
  void toggleLaser() { toggleFeature(USING_LASER); }
  void toggleHid() { toggleFeature(USING_HID); }
  void toggleMpu() { if(!isMotionDetectLocked()) toggleFeature(USING_MPU); }
  void toggleOTA() { toggleFeature(USING_OTA_UPLOAD); }
  
  bool isHapticsEnabled() const { return isFeatureEnabled(USING_HAPTICS); }
  bool isLaserEnabled() const { return isFeatureEnabled(USING_LASER); }
  bool isHidEnabled() const { return isFeatureEnabled(USING_HID); }
  bool isMpuEnabled() const { return isFeatureEnabled(USING_MPU); }
  bool isOTAEnabled() const { return isFeatureEnabled(USING_OTA_UPLOAD); }

  // Motion detect lock management
  void toggleMotionDetectLock() { 
    mpu_motion_detect_lock = !mpu_motion_detect_lock; 
    if(mpu_motion_detect_lock) {
      enableFeature(USING_MPU);
    } else {
      disableFeature(USING_MPU);
    }
  }
  bool isMotionDetectLocked() const { return mpu_motion_detect_lock; }
  
private:
  DeviceSettings device_settings;
  WifiSettings wifi_settings;
  EventGroupHandle_t device_function_event_group;
  uint8_t current_tuning_target; // 0=haptics, 1=laser, 2=led

  bool mpu_motion_detect_lock = false;

  void saveSettingsToPreferences(String target, uint8_t value);
  void loadSettingsFromPreferences(String target, uint8_t &value);
};

#endif // DEVICEMANAGER_H