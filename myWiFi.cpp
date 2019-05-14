#include <WiFi.h>

#include "mySerial.h"
#include "myWiFi.h"


void myWiFiConnect(char *ssid, char *password) {
  WiFi.begin(ssid, password);

  MY_INFO_PRINT("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    MY_INFO_PRINT(".");
    delay(500);
  }
  MY_INFO_PRINTLN();
  MY_INFO_PRINT("connected: ");
  MY_INFO_PRINTLN(WiFi.localIP());
}


void myWiFiCheckRestart() {
  if (WiFi.status() != WL_CONNECTED) {
    MY_INFO_PRINTLN("WiFi disconnected. Restart...");
    ESP.restart();
  }
}


IPAddress myWiFiLocalIP() {
  return WiFi.localIP();
}