#pragma once

#include <Arduino.h>

#define MY_WARN_ON
#ifdef MY_WARN_ON
#define MY_WARN_PRINT(...)    { Serial.print(__VA_ARGS__); Serial.flush(); }
#define MY_WARN_PRINTLN(...)  { Serial.println(__VA_ARGS__); Serial.flush(); }
#define MY_WARN_PRINTF(...)  { Serial.printf(__VA_ARGS__); Serial.flush(); }
#else
#define MY_WARN_PRINT(...)
#define MY_WARN_PRINTLN(...)
#define MY_WARN_PRINTF(...)
#endif

#define MY_INFO_ON
#ifdef MY_INFO_ON
#define MY_INFO_PRINT(...)    { Serial.print(__VA_ARGS__); Serial.flush(); }
#define MY_INFO_PRINTLN(...)  { Serial.println(__VA_ARGS__); Serial.flush(); }
#define MY_INFO_PRINTF(...)  { Serial.printf(__VA_ARGS__); Serial.flush(); }
#else
#define MY_INFO_PRINT(...)
#define MY_INFO_PRINTLN(...)
#define MY_INFO_PRINTF(...)
#endif

#define MY_DEBUG_ON
#ifdef MY_DEBUG_ON
#define MY_DEBUG_PRINT(...)    { Serial.print(__VA_ARGS__); Serial.flush(); }
#define MY_DEBUG_PRINTLN(...)  { Serial.println(__VA_ARGS__); Serial.flush(); }
#define MY_DEBUG_PRINTF(...)  { Serial.printf(__VA_ARGS__); Serial.flush(); }
#else
#define MY_DEBUG_PRINT(...)
#define MY_DEBUG_PRINTLN(...)
#define MY_DEBUG_PRINTF(...)
#endif
