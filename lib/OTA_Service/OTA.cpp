#include "OTA.h"

WebServer server(80);

void OTA::begin() {
  loadWiFiCredentials();
  if(WIFI_SSID == "") {
    Serial.println("No WiFi Credentials Found !");
    return;
  }
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
