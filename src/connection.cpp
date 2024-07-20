#include "app.h"
#include "config.h"
#include <WiFi.h>

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

void sendImageToServer(){
    http.begin(http_server)
}