#ifndef APP_H
#define APP_H

#include <WiFi.h>
#include <esp_camera.h>
#include <HTTPClient.h>
#include <stdint.h>


// From ultrasonic_sensor.cpp
void ultrasonic_setup();
float ultrasonic_get_distance();

// From camera.cpp 

// From connection.cpp
void checkWifiConnection();
void checkMqttConnection();
void sendImageToServer(const char* serverURL, camera_fb_t* fb);

#endif
