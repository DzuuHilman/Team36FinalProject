#include <driver/dac.h>
#include <HTTPClient.h>
#include <WiFi.h>

// Audio file
#define delay_sample 240         // Change according to sampling rate. value=60 for 16kbps bit rate (.wav) 
#define header_format_value 44  // Change according [to file type. value=44 for ".mp3" file

// Wifi credential
#define wifi_ssid "Hey hey"
#define wifi_pass "aingmaung"

// HTTP Server to get TTS
#define http_get_tts "http://192.168.161.107:5000/esp32/post_and_get_tts_voice"
HTTPClient http;

uint16_t data16;
uint8_t left;

uint16_t delayus = delay_sample;
uint8_t mp3HeaderSize = header_format_value;

// Declaration
void activateWifiConnection();
void playMp3File(uint8_t *buffPlay, int len);
void fetchAndPlayAudio();

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 Speaker begin!");

    activateWifiConnection();
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_voltage(DAC_CHANNEL_1, 0);
}

void loop () {
    fetchAndPlayAudio();
    delay(1000);
}

// Fetch and play audio from server
void fetchAndPlayAudio() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(http_get_tts);
    http.setTimeout(10000);           // Increase timeout
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

// Play mp3 to I2S Speaker
void playMp3File(uint8_t *buffPlay, int len) {
    for (int i = 0; i < len; i += sizeof(data16)) {
        memcpy(&data16, (char*)buffPlay + i, sizeof(data16));
        left = ((uint16_t)data16 + 32767) >> 8;
        dac_output_voltage(DAC_CHANNEL_1, left);
        delayMicroseconds(delayus);
    }
    
}

// Activate Wifi connection
void activateWifiConnection(){
    WiFi.begin(wifi_ssid, wifi_pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    

    Serial.println("");
    Serial.println("WiFi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP().toString());
}


