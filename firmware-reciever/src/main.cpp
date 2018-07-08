//slave/receiver  ESP-8266
#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
  #include <espnow.h>
}
#define CHANNEL 0


struct __attribute__((packed)) COMMAND {
  char commandType;
  float payload;
} commandData;

unsigned long timeout = millis();

// Init ESP Now with fallback
void InitESPNow() {
  if (esp_now_init() == 0) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

int ledPin = D1;
bool toggled = 0;

// callback when data is recv from Master
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  /*char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);*/
  Serial.print("Last Packet Recv Data: "); Serial.println(int(*data));
  Serial.println("");
  
  memcpy(&commandData, data, sizeof(commandData));

  Serial.printf(" CommandType=%c, Payload=%0.0f%%\n", commandData.commandType, commandData.payload); 
  switch(commandData.commandType){
    case 'b':
      toggled = !toggled;
      if(toggled){
        analogWrite(ledPin, 1023);
      }else{
        analogWrite(ledPin, 0);
      }
      
    break;
    case 'x':
      analogWrite(ledPin, int(commandData.payload)*5);
    break;
    case 'y':
      analogWrite(ledPin, int(commandData.payload)*5);
    break;
  }
}



void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("ESPNow Basic Slave Example");

  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);

  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP_STA);
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);
}



void loop() {
  /*if (millis() - timeout > 3000) {
    Serial.println("Waiting for data ...");
    timeout = millis();
  }*/
}