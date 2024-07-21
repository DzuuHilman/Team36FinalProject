#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <driver/dac.h>

// WiFi credential
#define wifi_ssid "Hey hey"                         // Change to current actived WiFi
#define wifi_pass "aingmaung"

// HTTP global variable
extern HTTPClient http;
#define http_post_server "http://192.168.161.107:5000/esp32/post_images"
#define http_get_tts "http://192.168.161.107:5000/esp32/post_and_get_tts_voice"

// Ultrasonic app
#define trigPin                       12
#define echoPin                       14
#define soundSpeed                    0.034            // Sound speed in cm/μs
#define range_minimum_camera_active   80               // Minimum distance in cm for camera being ON 

// Camera app...
// Camera Pins for ==[ ESP32 Wrover kit ]==
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  21
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27

#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    19
#define Y4_GPIO_NUM    18
#define Y3_GPIO_NUM    5
#define Y2_GPIO_NUM    4
#define VSYNC_GPIO_NUM 32       // Default is 25, but we want use that pin for DAC
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

// Speaker app...
#define delay_sample 60         // Change according to sampling rate. value=60 for 16kbps bit rate (.wav) 
#define header_format_value 44  // Change according to file type. value=44 for ".mp3" file


#endif
