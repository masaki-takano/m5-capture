#pragma once

#include <Arduino.h>

void myWiFiConnect(char *ssid, char *password);
void myWiFiCheckRestart();
IPAddress myWiFiLocalIP();
