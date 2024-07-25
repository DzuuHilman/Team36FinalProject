#include "app.h"
#include "config.h"

int distance = 0;

void setup() {
  Serial.begin(115200);
  // Set up ultrasonic sensor
  ultrasonic_setup();

  // set up camera configuration (see Camera app > Camera Pins in [config.h] file)
  camera_config_t config;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;  

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
  }
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }
  // Check WiFi connection (see [connection.cpp])
  checkWifiConnection();
}

void loop() {
  // Check WiFi is on
  if (!WiFi.isConnected()) {
    Serial.println("WiFi disconnected, reconnecting...");
    checkWifiConnection();
  }

  // Check if object is present in minimum range
  distance = ultrasonic_get_distance();
  if (distance < range_minimum_camera_active) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (fb) {
      sendImageToServer(http_post_server, fb);
      esp_camera_fb_return(fb);
    }
  } else {
    Serial.println("No object detected");
  }

  delay(100);
}