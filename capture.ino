#define WIFI_SSID       "XXXXXXXXX" // FIXME
#define WIFI_PASSWORD   "XXXXXXXXX" // FIXME

#define LINE_TOKEN    "XXXXXXXXX" // FIXME
#define LINE_MESSAGE  "Detected!"

#define GOOGLE_CLIENT_ID      "XXXXXXXXX.apps.googleusercontent.com"  // FIXME
#define GOOGLE_SECRET         "XXXXXXXXX"  // FIXME
#define GOOGLE_REFRESH_TOKEN  "XXXXXXXXX"  // FIXME
#define PARENT_ID             "XXXXXXXXX"  // FIXME

#define LINE_INTERVAL (60 * 1000)


#include "esp_camera.h"

#include "mySerial.h"
#include "myWiFi.h"


#define PIN_PIR_SIG 13  // Motion Sensor

#define LOOP_WAIT       100


bool initCamera();
void setFrameSize(framesize_t framesize);
bool lineNotify(String token, String message);
String googleGetAccessToken(
  String client_id,
  String client_secret,
  String refresh_token);
bool googleUpload(String token, String parent);


unsigned long detectInterval;


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  MY_DEBUG_PRINTLN();

  pinMode(PIN_PIR_SIG, INPUT);

  initCamera();

  //setFrameSize(FRAMESIZE_QQVGA);
  //setFrameSize(FRAMESIZE_QQVGA2);
  //setFrameSize(FRAMESIZE_QCIF);
  //setFrameSize(FRAMESIZE_HQVGA);
  //setFrameSize(FRAMESIZE_QVGA);
  //setFrameSize(FRAMESIZE_CIF);
  //setFrameSize(FRAMESIZE_VGA);
  //setFrameSize(FRAMESIZE_SVGA);
  setFrameSize(FRAMESIZE_XGA);    // Max for LINE
  //setFrameSize(FRAMESIZE_SXGA);
  //setFrameSize(FRAMESIZE_UXGA);

  myWiFiConnect(WIFI_SSID, WIFI_PASSWORD);

  detectInterval = 0;
}


void loop() {
  myWiFiCheckRestart();

  unsigned long t = millis();

  if (digitalRead(PIN_PIR_SIG) == HIGH) {

    if (detectInterval == 0) {
      // send to LINE at first time.
      lineNotify(LINE_TOKEN, LINE_MESSAGE);

      detectInterval = t + LINE_INTERVAL;

    } else {
      // send to GoogleDrive after the second.
      String token = googleGetAccessToken(
        GOOGLE_CLIENT_ID,
        GOOGLE_SECRET,
        GOOGLE_REFRESH_TOKEN);

      googleUpload(token, PARENT_ID);
    }

    delay(1000);
  }

  if (detectInterval < t) {
    detectInterval = 0;
  }

  delay(LOOP_WAIT);
}