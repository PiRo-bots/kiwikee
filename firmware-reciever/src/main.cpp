//slave/receiver  ESP-8266
#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
  #include <espnow.h>
}
#include <Wire.h>
#include <WEMOS_Motor.h>

#define CHANNEL 0


struct __attribute__((packed)) COMMAND {
  char commandType;
  float payload;
} commandData;

unsigned long timeout = millis();
unsigned long lastReading;

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

int ledPin = D4;
bool toggled = 0;
float pwm;

float multiplier = 75.0;

bool xChanged = false;
bool throttleChanged = false;

float prevX = 0.0;
float x=0.0;
float prevThrottle = 0.0;
float throttle=0.0;

float motorLeftPwm = 0.0;
float motorRightPwm = 0.0;
float motorLeftDir = _CW;
float motorRightDir = _CCW;

//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor *M1 = NULL;
Motor *M2 = NULL;


// callback when data is recv from Master
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  /*char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);*/
  // Serial.print("Last Packet Recv Data: "); Serial.println(int(*data));
  // Serial.println("");
  
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
      x = ((float(commandData.payload) - 512)/512);
      // Serial.print("x");
      // Serial.println(x);
      // analogWrite(ledPin, int(commandData.payload));
    break;
    case 'y':
      throttle = ((float(commandData.payload) - 512)/512) ;
      
      
      /*Serial.print("throttle");
      Serial.println(throttle);
      Serial.print("prev throttle");
      Serial.println(prevThrottle);*/
      // analogWrite(ledPin, int(commandData.payload));
    break;
  }
}



void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Reciever listening");

  pinMode(ledPin, OUTPUT);
  // analogWrite(ledPin, 0);
  M1 = new Motor(0x30,_MOTOR_A, 1000);//Motor A
  M2 = new Motor(0x30,_MOTOR_B, 1000);//Motor B

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

   /* for (pwm = 0; pwm <= 100; pwm++)
  {
    M1->setmotor( _CW, pwm);
    M2->setmotor(_CCW, pwm);
    Serial.printf("A:%d%, B:%d%, DIR:CW\r\n", pwm,100-pwm);
  }
  
  M1->setmotor(_STOP);
  M2->setmotor( _STOP);
  Serial.println("Motor A&B STOP");
  delay(200);
  
  for (pwm = 0; pwm <=100; pwm++)
  {
    M1->setmotor(_CCW, pwm);
    M2->setmotor(_CW, pwm);
    Serial.printf("A:%d%, B:%d%, DIR:CCW\r\n", pwm,100-pwm);

  }
  
  M1->setmotor(_STOP);
  M2->setmotor( _STOP);
  delay(200);
  Serial.println("Motor A&B STOP");*/
  if (millis () - lastReading >= 5)   // 200 Hz
  {
    lastReading = millis ();
    if(prevThrottle != throttle) {
      throttleChanged = true;
      Serial.println("y changed");

    }
    if(prevX != x ) {
      xChanged = true;
      Serial.println("x changed");
    }
    prevThrottle = throttle;
    prevX = x;

    if(throttleChanged || xChanged){
      if(throttle>5){
        motorLeftDir = _CW;
        motorRightDir = _CCW;
      }else if(throttle<-5){
        motorLeftDir = _CCW;
        motorRightDir = _CW;
      }else{
        throttle = 0;
        prevThrottle = 0;
        // Serial.println("stopping");
        M1->setmotor( motorLeftDir, 0);
        M2->setmotor( motorRightDir, 0);
        M1->setmotor(_STOP);
        M2->setmotor(_STOP);
      }
      /*Serial.print("throttle: ");
      Serial.println(throttle);
      Serial.print("motorLeftDir: ");
      Serial.println(motorLeftDir);
      Serial.print("motorRightDir: ");
      Serial.println(motorRightDir);*/

      motorLeftPwm = max( float(abs(throttle) + x), float(0)) * multiplier;
      motorRightPwm = max( float(abs(throttle) - x), float(0)) * multiplier;
      M1->setmotor( motorLeftDir, throttle);
      M2->setmotor( motorRightDir, throttle);

      Serial.print("motorLeftPwm: ");
      Serial.println(motorLeftPwm);
      Serial.print("motorRightPwm: ");
      Serial.println(motorRightPwm);
    }
    throttleChanged = false;
  }
}