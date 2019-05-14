#include "mySerial.h"
#include "esp_camera.h"
#include <WiFiClientSecure.h>


#define API_HOST        "notify-api.line.me"
#define API_PORT        443
#define API_PATH_NOTIFY "/api/notify"


String readRespons(WiFiClientSecure *client);
void setFrameSize(framesize_t framesize);
void myClientWrite(
  WiFiClientSecure *client,
  const uint8_t *buf,
  size_t size);


bool lineApiNotify(
  String method,
  String host,
  String path,
  String token,
  String message,
  camera_fb_t * fb) {

  WiFiClientSecure client;

  if (!client.connect(host.c_str(), API_PORT)) {
    MY_INFO_PRINTLN("Connection failed!");
    return false;
  }

  String req_url_str;
  req_url_str = method + " " + path + " HTTP/1.1\r\n";

  String boundary = "------------------------665715c39c6ed91a";

  int contentLength = 0;

  String body = "--" + boundary + "\r\n";
  body += "Content-Disposition: form-data; name=\"message\"\r\n";
  body += "\r\n";
  body += message + "\r\n";
  contentLength += body.length();

  if (fb != NULL) {
    String imgBody = "--" + boundary + "\r\n";
    imgBody += "Content-Disposition: form-data; name=\"imageFile\"; filename=\"capture.jpg\"\r\n";
    imgBody += "Content-Type: image/jpeg\r\n";
    imgBody += "\r\n";
    contentLength += imgBody.length() + fb->len + strlen("\r\n");
    body += imgBody;
  }

  String lastLine = "--" + boundary + "--\r\n";
  contentLength += lastLine.length();

  String head;
  head = "Host: " + host + "\r\n";
  head += "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";
  head += "Content-Length: " + String(contentLength) + "\r\n";
  head += "Authorization: Bearer " + token + "\r\n";
  head += "Connection: Close\r\n";
  head += "\r\n"; //空行

  client.print(req_url_str);
  MY_WARN_PRINT(req_url_str);

  client.print(head);
  MY_WARN_PRINT(head);

  client.print(body);
  MY_WARN_PRINT(body);

  if (fb != NULL) {
    myClientWrite(&client, (const uint8_t *)fb->buf, fb->len);
    MY_WARN_PRINTF("LEN: %d\n", fb->len);
    client.print("\r\n");
  }

  client.print(lastLine);
  MY_WARN_PRINT(lastLine);

  client.flush();

  readRespons(&client);

  client.stop();

  return true;
}


bool lineNotify(String token, String message) {
  camera_fb_t *fb = esp_camera_fb_get();

  MY_WARN_PRINTF("WIDTH: %d\n", fb->width);

  bool send = lineApiNotify(
    "POST",
    API_HOST,
    API_PATH_NOTIFY,
    token,
    message,
    fb);

  esp_camera_fb_return(fb);

  return send;
}
