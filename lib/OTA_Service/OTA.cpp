#include "OTA.h"

WebServer server(80);

static const char* WIFI_SSID = "Tu123";
static const char* WIFI_PASS = "09090909";

void OTA::begin() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI Connecting...");
    delay(300);
  }

  Serial.println("WiFi Connected !!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("OTA Upload Here: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/update");
    
  ElegantOTA.begin(&server);
  server.begin();
}

void OTA::loop() {
  server.handleClient();
  ElegantOTA.loop();
}
