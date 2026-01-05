#ifndef OTA_H
#define OTA_H

#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>

class OTA {
public:
  void begin();
  void loop();
};

#endif // OTA_H