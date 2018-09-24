#include <Servo.h>

Servo grabberServo;
int grabberPos = 0;    // variable to store the servo position

void setupGrabber(int pin){
  grabberServo.attach(pin);
}

// callback when data is recv from Master
void OnDataRecvGrabber(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  memcpy(&commandData, data, sizeof(commandData));

  //Serial.println(commandData.payload);
  // Serial.printf(" CommandType=%c, Payload=%0.0f%%\n", commandData.commandType, commandData.payload); 
  switch(commandData.commandType){
    case 'y':
      
      //
      grabberPos = ((float(commandData.payload))/512) * 90 + 90;
      Serial.println(commandData.payload);
      Serial.println(grabberPos);
      grabberServo.write(grabberPos);  
    break;
  }
}