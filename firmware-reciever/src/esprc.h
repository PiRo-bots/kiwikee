#include <ESP8266WiFi.h>

extern "C" {
#include <espnow.h>
}

#define WIFI_CHANNEL 0

#define MESSAGELEN 10

struct __attribute__((packed)) COMMAND {
  char commandType;
  float payload;
} commandData;

volatile boolean callbackCalled;

// Init ESP Now with fallback
void setupEspNow() {
  while(esp_now_init() != 0){
    if (esp_now_init() == 0) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
  }
  
}




