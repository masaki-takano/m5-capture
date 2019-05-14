#include "mySerial.h"
#include <WiFiClientSecure.h>


String readRespons(WiFiClientSecure *client) {

  String res = "";

  if (client->connected()) {
    MY_INFO_PRINTLN("------------- Start Read Response Headers...");
    for (;;) {
      String head = client->readStringUntil('\n');
      MY_INFO_PRINTLN(head);
      if (head == "\r") {
        break;
      }
    }
    MY_INFO_PRINTLN("------------- Response Headers Received.");

    while (client->available()) {
      char c = client->read();
      res += c;
      MY_INFO_PRINT(c);
    }
    MY_INFO_PRINTLN("------------- Body Received.");

  } else {

    MY_INFO_PRINTLN("------------- Not Connected.");

  }

  return res;
}
