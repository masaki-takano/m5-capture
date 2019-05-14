#include "mySerial.h"
#include "esp_camera.h"

#include <ArduinoJson.h>
#include <WiFiClientSecure.h>


#define API_HOST    "www.googleapis.com"
#define API_PORT    443
#define API_PATH_TOKEN   "/oauth2/v4/token"
#define API_PATH_UPLOAD  "/upload/drive/v3/files?uploadType=multipart"


String readRespons(WiFiClientSecure *client);
void setFrameSize(framesize_t framesize);
void myClientWrite(
  WiFiClientSecure *client,
  const uint8_t *buf,
  size_t size);


#define MY_JSON_MSG_MAX_LENGTH  512

StaticJsonBuffer<MY_JSON_MSG_MAX_LENGTH> jsonBuffer;


String googleGetAccessToken(
  String client_id,
  String client_secret,
  String refresh_token) {

  String host = API_HOST;
  int port = API_PORT;
  String path = API_PATH_TOKEN;

  WiFiClientSecure client;

  if (!client.connect(host.c_str(), port)) {
    MY_INFO_PRINTLN("Connection failed!");
    return "";
  }

  String body = "refresh_token=" + refresh_token;
  body += "&client_id=" + client_id;
  body += "&client_secret=" + client_secret;
  body += "&grant_type=refresh_token";

  String req_url_str = "POST " + path + " HTTP/1.1\r\n";

  String head = "Host: " + host + ":" + String(port) + "\r\n";
  head += "Connection: close\r\n";
  head += "Content-Type: application/x-www-form-urlencoded\r\n";
  head += "Content-Length: ";
  head += String(body.length());
  head += "\r\n\r\n";

  client.print(req_url_str);
  MY_WARN_PRINTLN(req_url_str);

  client.print(head);
  MY_WARN_PRINTLN(head);

  client.print(body);
  MY_WARN_PRINTLN(body);

  String res = readRespons(&client);

  client.stop();

  JsonObject& root = jsonBuffer.parseObject(res);

  const char *dataString = NULL;
  if (root.is<char *>("access_token")) {
    dataString = root["access_token"].as<char *>();
    MY_INFO_PRINTLN(dataString);
  }

  jsonBuffer.clear();

  return dataString != NULL ? String(dataString) : "";
}


bool googleUploadImage(
  String method,
  String host,
  String path,
  String token,
  String fileInfo,
  camera_fb_t * fb) {

  WiFiClientSecure client;

  if (!client.connect(host.c_str(), 443)) {
    MY_INFO_PRINTLN("Connection failed!");
    return false;
  }

  String req_url_str;
  req_url_str = method + " " + path + " HTTP/1.1\r\n";

  String boundary = "------------------------665715c39c6ed91a";

  int contentLength = 0;

  String body = "--" + boundary + "\r\n";
  body += "Content-Type: application/json; charset=UTF-8\r\n";
  body += "\r\n";
  body += fileInfo + "\r\n";
  contentLength += body.length();

  String imgBody = "--" + boundary + "\r\n";
  imgBody += "Content-Type: image/jpeg\r\n";
  imgBody += "\r\n";
  contentLength += imgBody.length() + fb->len + strlen("\r\n");
  body += imgBody;

  String lastLine = "--" + boundary + "--\r\n";
  contentLength += lastLine.length();

  String head;
  head = "Host: " + host + "\r\n";
  head += "Content-Type: multipart/related; boundary=" + boundary + "\r\n";
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

  myClientWrite(&client, (const uint8_t *)fb->buf, fb->len);
  MY_WARN_PRINTF("LEN: %d\n", fb->len);
  client.print("\r\n");

  client.print(lastLine);
  MY_WARN_PRINT(lastLine);

  client.flush();

  readRespons(&client);

  client.stop();

  return true;
}


bool googleUpload(String token, String parent) {
    camera_fb_t *fb = esp_camera_fb_get();

    MY_WARN_PRINTF("WIDTH: %d\n", fb->width);

    String fileInfo = "{\"name\":\"test.jpg\",\"parents\":[\"" + parent + "\"]}";
    MY_WARN_PRINTLN("fileInfo: " + fileInfo);

    bool send = googleUploadImage(
        "POST",
        API_HOST,
        API_PATH_UPLOAD,
        token,
        fileInfo,
        fb);

    esp_camera_fb_return(fb);

    return send;
}
