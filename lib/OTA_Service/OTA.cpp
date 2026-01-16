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
  Serial.println("Device is going to restart...");
  delay(1000);
  ESP.restart();

  // TODO: Properly stop OTA service(can't turn off WiFi due to conflicts with BLE HID)
//   server.stop();
//   delay(100);  // Allow server to fully stop
// #ifdef USING_WIFI_STA
//   WiFi.disconnect();  // Disconnect without turning off radio first
//   delay(100);
//   WiFi.mode(WIFI_OFF);  // Then turn off WiFi
// #endif  // USING_WIFI_STA
// #ifdef USING_WIFI_AP
//   WiFi.softAPdisconnect();  // Disconnect without turning off radio first
//   delay(100);
//   WiFi.mode(WIFI_OFF);  // Then turn off WiFi
// #endif  // USING_WIFI_AP
//   Serial.println("OTA Service Stopped !");
}

void OTA::WiFiSTAMode() {
  WiFi.mode(WIFI_STA);  // Enable STA mode (also powers on radio)
  delay(100);
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
  WiFi.mode(WIFI_AP);  // Enable AP mode (also powers on radio)
  delay(100);
  WiFi.softAP("Tu's Wand", "Tuzi9227");        // TODO: Make configurable  
  Serial.println("WiFi AP Mode Started !!");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("OTA Upload Here: ");
  Serial.print("http://");
  Serial.print(WiFi.softAPIP());
  Serial.println("/update");
}