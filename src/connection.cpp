#include <string>
#include "HardwareSerial.h"
#include "app.h"
#include "config.h"

HTTPClient http;
BluetoothSerial SerialBT;

void checkWifiConnection(){
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
void checkBluetoothConnection(char bt_name){
    SerialBT.begin(bt_name);
    if(!SerialBT.begin(bt_name)){
        Serial.println("Failed to connect Bluetooth");
        return;
    }
    Serial.print(bt_name);
    Serial.println(" suscsessfully connect!");
}

char getBluetoothName(){
  // Waiting to start type in Serial Monitor
  char temp_bt_name;
  Serial.println("Please input your Bluetooth device name: ");
  while (Serial.available() == 0){

  }
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') break;  // Break on newline
    temp_bt_name += c;
  }
  return temp_bt_name;
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
        Serial.println("Response" + response);
    } else {
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}