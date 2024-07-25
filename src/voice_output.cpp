#include <stdint.h>
// #include "esp32-hal.h"
#include "app.h"
#include "config.h"

BluetoothA2DPSource a2dp_source;
int16_t data16;

uint16_t delayus = delay_sample;
// uint8_t mp3HeaderSize = header_format_value;

void playMp3File(uint8_t *buffPlay, uint32_t len) {

    SoundData *data = new OneChannel8BitSoundData((uint8_t*) buffPlay, len);

    // Send data over Bluetooth
    a2dp_source.set_volume(20);
    a2dp_source.write_data(data); 
    
    // Clean up memory
    delete data;    
}

void fetchAndPlayAudio() {
  if (http.connected()) http.end();

  if (WiFi.status() == WL_CONNECTED) {
    // Connect to TTS server
    http.begin(http_get_tts);
    Serial.println("Attemtping to get audio from TTS server...");
    int httpResponseCode = http.GET();

    // Check if HTTP is connected
    if (httpResponseCode == HTTP_CODE_OK) {
      int len = http.getSize();
      uint8_t *buff = http.getStream();

      playMp3File(buff, len);

      Serial.println("Finished playing TTS voice.");
    } else {
      Serial.printf("Error to getting audio from TTS server: %s (", http.errorToString(httpResponseCode).c_str());
      Serial.printf("%i)\n", httpResponseCode);
    } 
  } else {
    Serial.println("Error in WiFi Connection");
  }

  http.end();
}