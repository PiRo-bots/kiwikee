//slave/receiver  ESP-8266
/*extern "C" {
  #include "user_interface.h"
  #include <espnow.h>
}*/
#include "config.h"
#include "esprc.h"
#include "ledFace.h"

unsigned long timeout = millis();
unsigned long lastReading;

#include "drive.h"

/*void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  
}*/

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Reciever listening");

  // pinMode(ledPin, OUTPUT);

  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP_STA);
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());

  // Init ESPNow with a fallback logic
  setupEspNow();

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecvMotor);

  delay(2000);
  // setup drive mode
  setupDrive();
  // setup led matrix 'face'
  setupFace();

  ///
  // displayFace(0);
}


void loop() {

  drive();
  displayFace(throttle);
}