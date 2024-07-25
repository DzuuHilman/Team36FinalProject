 #include "esp32-hal.h"
#include "app.h"
#include "config.h"


uint16_t data16;
uint8_t left;

uint16_t delayus = delay_sample;
uint8_t mp3HeaderSize = header_format_value;

void playMp3File(uint8_t *buffPlay, int len) {
    for (int i = 0; i < len; i += sizeof(data16)) {
        memcpy(&data16, (char*)buffPlay + i, sizeof(data16));
        left = ((uint16_t)data16 + 32767) >> 8;
        dac_output_voltage(DAC_CHANNEL_1, left);
        delayMicroseconds(delayus);
    }
    
}

void fetchAndPlayAudio() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(http_get_tts);
    int httpResponseCode = http.GET();

    // Check if HTTP is connected
    if (httpResponseCode == HTTP_CODE_OK) {
      int len = http.getSize();
      uint8_t buff[128] = {0};
      int buffPoint = 0;

      WiFiClient* stream = http.getStreamPtr();
      while (http.connected() && (len > 0 || len == -1)) {
        size_t size = stream -> available();

        if (size > 0) {
          int buffLeft = sizeof(buff) - buffPoint;
          int c = stream->readBytes(buff + buffPoint, ((size > buffLeft) ? buffLeft : size));
          buffPoint += c;

          if (buffPoint >= sizeof(buff)) {
            playMp3File(buff, buffPoint);
            buff[0] = buff[buffPoint - 1];
            buffPoint = buffPoint % sizeof(data16);
          }

        }

      }

      if (buffPoint > sizeof(data16)) {
        playMp3File(buff, buffPoint);
      }
      Serial.println("Finished playing TTS voice.");
    }
    else {
    Serial.printf("Failed on HTTP GET. Error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end(); 
  }
  else {
    Serial.println("Error in WiFi Connection");
    http.end();
  }

  dac_output_voltage(DAC_CHANNEL_1, 0);

}