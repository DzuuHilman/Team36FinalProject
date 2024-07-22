#include "WString.h"
#include <string>
#include "HardwareSerial.h"
#include "app.h"
#include "config.h"

HTTPClient http;
BluetoothSerial SerialBT;

// For Bluetooth 
bool connected;


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

void startBluetoothConnection(){
  String master = bt_master_name;
    if(!SerialBT.begin(master, true)){
        Serial.println("Failed to connect Bluetooth");
        return;
    }
    Serial.print(master);
    Serial.println(" is started in bluetooth master mode.");
}

void attemptToConnectSlaveBluetooth(){
  String slave = bt_slave_name;
  connected = SerialBT.connect(slave);
  
  Serial.printf("Connecting to slave BT device named \"%s\"\n", slave.c_str());

  // Check if connection is sucsees
  if (connected) {
    Serial.println("Connected sucsessfully!");
  } else {
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    }
  }
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
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}