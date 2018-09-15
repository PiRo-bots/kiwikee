int ledPin = D4;
bool toggled = 1;

void setupStatusLight(){
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 1023);
}
// callback when data is recv from Master
void OnDataRecvStatusLight(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  memcpy(&commandData, data, sizeof(commandData));

  // Serial.printf(" CommandType=%c, Payload=%0.0f%%\n", commandData.commandType, commandData.payload); 
  switch(commandData.commandType){
    case 'b':
      toggled = !toggled;
      if(toggled){
        analogWrite(ledPin, 1023);
      }else{
        analogWrite(ledPin, 0);
      }
    break;
  }
}