#include "OTA.h"

WebServer server(80);

void OTA::begin() {
  loadWiFiCredentials();
  if(WIFI_SSID == "") {
    Serial.println("No WiFi Credentials Found !");
    return;
  }

#ifdef USING_WIFI_STA
  WiFiSTAMode();
#endif  // USING_WIFI_STA
#ifdef USING_WIFI_AP
  WiFiAPMode();
#endif  // USING_WIFI_AP    

  ElegantOTA.begin(&server);
  server.begin();
}

void OTA::loop() {
  server.handleClient();
  ElegantOTA.loop();
}

void OTA::end() {
  server.stop();
#ifdef USING_WIFI_STA
  WiFi.disconnect(true, true);
#endif  // USING_WIFI_STA
#ifdef USING_WIFI_AP
  WiFi.softAPdisconnect(true);
#endif  // USING_WIFI_AP
}

void OTA::WiFiSTAMode() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI Connecting...");
    delay(50);
  }
  Serial.println("WiFi Connected !!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("OTA Upload Here: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/update");
}

void OTA::WiFiAPMode() {
  WiFi.softAP("Tu's Wand", "Tuzi9227");        // TODO: Make configurable  
  Serial.println("WiFi AP Mode Started !!");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("OTA Upload Here: ");
  Serial.print("http://");
  Serial.print(WiFi.softAPIP());
  Serial.println("/update");
}