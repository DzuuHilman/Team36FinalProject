/*
#include "config.h"
#include "app.h"
#include <Arduino.h>


void checkCameraIsFree() {
  if (camera.status() == FRAMING) {
    Serial.println("Camera is busy");
    delay(100);
  }
}

void camera_setup() {
  camera.begin(resolution);
  camera.setFormat(format);
  camera.setQuality(quality);
  camera.setFlip(flip);
  camera.setMirror(mirror);
  camera.setJPEGQuality(jpegQuality);
}

void camera_loop() {
  checkCameraIsFree();
  camera.capture(buffer, sizeof(buffer));
  Serial.println("Camera captured");
}
*/