#include "WString.h"
#ifndef APP_H
#define APP_H

#include <esp_camera.h>


// From ultrasonic_sensor.cpp
void ultrasonic_setup();
float ultrasonic_get_distance();

// From camera.cpp 

// From connection.cpp
void checkWifiConnection();
int checkEspMemory();
void sendImageToServer(const char* serverURL, camera_fb_t* fb);

// From voice_output.cpp
void playMp3File(uint8_t *buffPlay, int32_t len);
void fetchAndPlayAudio();

#endif
