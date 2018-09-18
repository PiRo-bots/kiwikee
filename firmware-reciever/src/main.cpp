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

// robot modes:
// 0: RC
// 1: autonomous move around with ultrasonic distance sensor
int mode = 0;



// callback when data is recv from Master
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  memcpy(&commandData, data, sizeof(commandData));

  switch(commandData.commandType){
    case 'm':// mode switch
      mode = int(commandData.payload);
      if(mode == 1){
        Serial.println("Switching to atonomous mode");
      } 
    break;
  }
  if(mode == 0){
    OnDataRecvStatusLight(mac_addr,  data, len);
    OnDataRecvMotor(mac_addr,  data, len);
  }
}


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

long lastCheck = millis();
 
void loop() {

  drive();
  displayFace(throttle, distance);
  // measureDistance();

  if(mode == 1) {
     if (millis () - lastCheck >= 100)
    {
      lastCheck = millis ();
      if(measureDistance()<15){
        Serial.println("too close !");
        x = -0.8;
        throttle = - 0.8;
      }else{
        Serial.println("all fine !");
        x = 0;
        throttle = 0.7;
      }
    }
   
  }

  // analogWrite(A0, 255);
  //analogWrite(D4, 255);
  //fooOut++;
}