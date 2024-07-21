#ifndef APP_H
#define APP_H

#include <WiFi.h>
#include <esp_camera.h>
#include <HTTPClient.h>
#include "HardwareSerial.h"
#include <stdint.h>


// From ultrasonic_sensor.cpp
void ultrasonic_setup();
float ultrasonic_get_distance();

// From camera.cpp 

// From connection.cpp
void checkWifiConnection();
void sendImageToServer(const char* serverURL, camera_fb_t* fb);

// From tts.cpp
void playMp3File(uint8_t *buffPlay, int len);
void fetchAndPlayAudio();

#endif
