#ifndef OTA_H
#define OTA_H

#include "Utils.h"
#include <ElegantOTA.h>
#include <Preferences.h>

class OTA {
public:
  void begin();
  void loop();
  void end();

  void saveWiFi(const char* ssid, const char* pass) {
    prefs.begin("wifi", false);  // namespace = "wifi"
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.end();
  }

private:
  Preferences prefs;
  String WIFI_SSID, WIFI_PASS;
  
  void loadWiFiCredentials() {
    prefs.begin("wifi", true);  // namespace = "wifi"
    WIFI_SSID = prefs.getString("ssid", "");
    WIFI_PASS = prefs.getString("pass", "");
    prefs.end();
  }

  void WiFiSTAMode();
  void WiFiAPMode();
};

#endif // OTA_H