#ifndef OTA_H
#define OTA_H

#include "Utils.h"
#include <ElegantOTA.h>

class OTA {
public:
  void begin();
  void loop();
  void end();

  void saveWiFi(const char* ssid, const char* pass) {
    Preferences prefs;
    prefs.begin("wifi", false);  // namespace = "wifi"
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.end();
  }

private:
  String WIFI_SSID, WIFI_PASS;
  
  // TODO: Load WiFi credentials from DeviceManager settings
  void loadWiFiCredentials() {
    Preferences prefs;
    prefs.begin("wifi", true);  // namespace = "wifi"
    WIFI_SSID = prefs.getString("ssid", "");
    WIFI_PASS = prefs.getString("pass", "");
    prefs.end();
  }

  void WiFiSTAMode();
  void WiFiAPMode();
};

#endif // OTA_H