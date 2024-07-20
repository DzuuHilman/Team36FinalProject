#include "WString.h"
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
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP().toString());
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