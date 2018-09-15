//slave/receiver  ESP-8266
/*extern "C" {
  #include "user_interface.h"
  #include <espnow.h>
}*/
// unsigned long timeout = millis();

#include "config.h"
#include "esprc.h"
#include "ledFace.h"
#include "drive.h"
#include "statusLight.h"
#include "sensor-distance.h"

// callback when data is recv from Master
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  // memcpy(&commandData, data, sizeof(commandData));

  OnDataRecvStatusLight(mac_addr,  data, len);
  OnDataRecvMotor(mac_addr,  data, len);
}

int fooOut = 0;


void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Reciever listening");

  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP_STA);
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());

  // Init ESPNow with a fallback logic
  setupEspNow();

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);
  // esp_now_register_recv_cb(OnDataRecvStatusLight);


  delay(2000);
  // setup drive mode
  setupDrive();
  // setup led matrix 'face'
  setupFace();
  // turn the light on, so we now we are turned on
  setupStatusLight();
  // 
  setupDistanceSensor();
  ///
  // pinMode(A0, OUTPUT);
  // pinMode(D4, OUTPUT);
}


void loop() {

  drive();
  displayFace(throttle, distance);
  // measureDistance();

  // analogWrite(A0, 255);
  //analogWrite(D4, 255);
  //fooOut++;
}