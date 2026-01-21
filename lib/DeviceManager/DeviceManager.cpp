#include "DeviceManager.h"
#include "Utils.h"

DeviceManager::DeviceManager()
  : device_settings(), current_tuning_target(0) {
  // Create the event group
  device_function_event_group = xEventGroupCreate();
  
  // Enable default features
  if (device_function_event_group != NULL) {
    xEventGroupSetBits(device_function_event_group, USING_HAPTICS | USING_LASER | USING_HID);
  }
}

void DeviceManager::begin() {
  // Load settings from Preferences
  loadSettingsFromPreferences("haptics", device_settings.hapticsIntensity);
  loadSettingsFromPreferences("laser", device_settings.laserIntensity);
  loadSettingsFromPreferences("led", device_settings.ledIntensity);
}

void DeviceManager::increaseSetting(uint8_t setting_id, uint8_t step) {
  uint8_t new_value;
  switch(setting_id) {
    case 0: // Haptics
      new_value = device_settings.hapticsIntensity + step;
      device_settings.hapticsIntensity = (new_value > 100) ? 100 : new_value;
      DEBUG_PRINT("Haptics Intensity: ");
      DEBUG_PRINTLN(device_settings.hapticsIntensity);
      saveSettingsToPreferences("haptics", device_settings.hapticsIntensity);
      break;
    case 1: // Laser
      new_value = device_settings.laserIntensity + step;
      device_settings.laserIntensity = (new_value > 100) ? 100 : new_value;
      DEBUG_PRINT("Laser Intensity: ");
      DEBUG_PRINTLN(device_settings.laserIntensity);
      saveSettingsToPreferences("laser", device_settings.laserIntensity);
      break;
    case 2: // LED
      new_value = device_settings.ledIntensity + step;
      device_settings.ledIntensity = (new_value > 100) ? 100 : new_value;
      DEBUG_PRINT("LED Intensity: ");
      DEBUG_PRINTLN(device_settings.ledIntensity);
      saveSettingsToPreferences("led", device_settings.ledIntensity);
      break;
    default:
      break;
  }
}

void DeviceManager::decreaseSetting(uint8_t setting_id, uint8_t step) {
  int16_t new_value;
  switch(setting_id) {
    case 0: // Haptics
      new_value = device_settings.hapticsIntensity - step;
      device_settings.hapticsIntensity = (new_value < 0) ? 0 : new_value;
      DEBUG_PRINT("Haptics Intensity: ");
      DEBUG_PRINTLN(device_settings.hapticsIntensity);
      saveSettingsToPreferences("haptics", device_settings.hapticsIntensity);
      break;
    case 1: // Laser
      new_value = device_settings.laserIntensity - step;
      device_settings.laserIntensity = (new_value < 0) ? 0 : new_value;
      DEBUG_PRINT("Laser Intensity: ");
      DEBUG_PRINTLN(device_settings.laserIntensity);
      saveSettingsToPreferences("laser", device_settings.laserIntensity);
      break;
    case 2: // LED
      new_value = device_settings.ledIntensity - step;
      device_settings.ledIntensity = (new_value < 0) ? 0 : new_value;
      DEBUG_PRINT("LED Intensity: ");
      DEBUG_PRINTLN(device_settings.ledIntensity);
      saveSettingsToPreferences("led", device_settings.ledIntensity);
      break;
    default:
      break;
  }
}

void DeviceManager::printCurrentSettings() {
  DEBUG_PRINTLN("\n========== Device Settings ==========");
  DEBUG_PRINT("Haptics Intensity: ");
  DEBUG_PRINTLN(device_settings.hapticsIntensity);
  DEBUG_PRINT("Laser Intensity: ");
  DEBUG_PRINTLN(device_settings.laserIntensity);
  DEBUG_PRINT("LED Intensity: ");
  DEBUG_PRINTLN(device_settings.ledIntensity);
  DEBUG_PRINTLN("=====================================\n");
}


void DeviceManager::enableFeature(EventBits_t feature_bit) {
  if (device_function_event_group != NULL) {
    xEventGroupSetBits(device_function_event_group, feature_bit);
  }
}

void DeviceManager::disableFeature(EventBits_t feature_bit) {
  if (device_function_event_group != NULL) {
    xEventGroupClearBits(device_function_event_group, feature_bit);
  }
}

void DeviceManager::toggleFeature(EventBits_t feature_bit) {
  if (device_function_event_group != NULL) {
    if(xEventGroupGetBits(device_function_event_group) & feature_bit) {
      xEventGroupClearBits(device_function_event_group, feature_bit);
    } else {
      xEventGroupSetBits(device_function_event_group, feature_bit);
    }
  }
}

bool DeviceManager::isFeatureEnabled(EventBits_t feature_bit) const {
  if (device_function_event_group != NULL) {
    return (xEventGroupGetBits(device_function_event_group) & feature_bit);
  }
  return false;
}

EventBits_t DeviceManager::getEnabledFeatures() const {
  if (device_function_event_group != NULL) {
    return xEventGroupGetBits(device_function_event_group);
  }
  return 0;
}

void DeviceManager::saveSettingsToPreferences(String target, uint8_t value) {
  Preferences prefs;
  prefs.begin("device_settings", false);
  prefs.putUChar(target.c_str(), value);
  prefs.end();
  DEBUG_PRINT("Saved ");
  DEBUG_PRINT(target);
  DEBUG_PRINT(": ");
  DEBUG_PRINTLN(value);
}

void DeviceManager::loadSettingsFromPreferences(String target, uint8_t &value) {
  Preferences prefs;
  prefs.begin("device_settings", true);
  value = prefs.getUChar(target.c_str(), 100);
  prefs.end();
  DEBUG_PRINT("Loaded ");
  DEBUG_PRINT(target);
  DEBUG_PRINT(": ");
  DEBUG_PRINTLN(value);
}