#include <WiFiClientSecure.h>

#define WRITE_SIZE_MAX   1024


void myClientWrite(
  WiFiClientSecure *client,
  const uint8_t *buf,
  size_t size
) {
  for (size_t total = 0, len = 0; total < size; total += len) {
    len = client->write(
      buf + total,
      (size - total >= WRITE_SIZE_MAX) ? WRITE_SIZE_MAX : (size - total)
    );
    client->flush();
  }
}
