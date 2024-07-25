#include "Esp.h"
#include "WiFi.h"
#include "WString.h"
#include <string>
#include "HardwareSerial.h"
#include "app.h"
#include "config.h"

HTTPClient http;

void checkWifiConnection(){

  WiFi.begin(wifi_ssid, wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());
}

char getBluetoothName(){
  // Waiting to start type in Serial Monitor
  char temp_bt_name;

  Serial.println("Please input your Bluetooth device number: ");
  while (Serial.available() == 0) {

  }
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') break;  // Break on newline
    temp_bt_name += c;
  }
  return temp_bt_name;
}

int checkEspMemory(){
  int memory = ESP.getFreeHeap();
  return memory;
} 
void sendImageToServer(const char* serverURL, camera_fb_t* fb){
    if(!http.begin(serverURL)){
      Serial.println("Failed to connect to server. Try again in 5 seconds.");
      return;
    }
    
    http.addHeader("Content-Type", "image/jpeg");

    int httpResponseCode = http.POST(fb->buf, fb->len);

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Response " + response);
    } else {
        Serial.print("Error on sending image to server: ");
        Serial.printf("%s (", http.errorToString(httpResponseCode).c_str());
        Serial.printf("%i) \n", httpResponseCode);
    }

    http.end();
}