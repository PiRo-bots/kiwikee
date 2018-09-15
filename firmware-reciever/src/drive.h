#include <Wire.h>
#include <WEMOS_Motor.h>

float pwm;

float multiplier = 100.0;
float maxSpeed = 75.0;

bool xChanged = false;
bool throttleChanged = false;

float prevX = 0.0;
float x=0.0;

float prevThrottle = 0.0;
float throttle=0.0;
float deadZoneThreshold = 3;

float motorLeftPwm = 0.0;
float motorRightPwm = 0.0;
float motorLeftDir = _CW;
float motorRightDir = _CW; // _CCW; // inverted compared to original firmware

//Motor shield I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor *motorLeft = new Motor(0x30,_MOTOR_A, 1000);//Motor Left //NULL;
Motor *motorRight = new Motor(0x30,_MOTOR_B, 1000);//Motor Right //NULL;

unsigned long lastReading;


void setupDrive() {
  // motorLeft = new Motor(0x30,_MOTOR_A, 1000);//Motor Left
  // motorRight = new Motor(0x30,_MOTOR_B, 1000);//Motor Right
}

void drive()
{
    if (millis () - lastReading >= 5)   // 200 Hz
    {
      lastReading = millis ();

      if(prevThrottle != throttle) {
      throttleChanged = true;
    }
    if(prevX != x ) {
      xChanged = true;
    }
    prevThrottle = throttle;
    prevX = x;

    if(throttleChanged || xChanged){
      float leftAdjuster = 1 - max(float(0), float(0 - x));
      float rightAdjuster = 1 - max(float(0), x);

      motorLeftPwm =  throttle * multiplier * leftAdjuster;
      motorRightPwm = throttle * multiplier * rightAdjuster;

      float motorLeftPwmFinal = min(maxSpeed, float(abs(motorLeftPwm)));
      float motorRightPwmFinal = min(maxSpeed, float(abs(motorRightPwm)));
      
      /*Serial.print("x: ");
      Serial.println(x);
      Serial.print("throttle: ");
      Serial.println(throttle);

      Serial.print("motorLeftPwm: ");
      Serial.println(abs(motorLeftPwm));

      Serial.print("motorRightPwm: ");
      Serial.println(abs(motorRightPwm));

      Serial.print("motorLeftPwmFinal: ");
      Serial.println(abs(motorLeftPwmFinal));

      Serial.print("motorRightPwmFinal: ");
      Serial.println(abs(motorRightPwmFinal));*/
     
      // left motor
      if(motorLeftPwm > deadZoneThreshold){
        motorLeftDir = _CCW;
      }else if(motorLeftPwm < -deadZoneThreshold){
        motorLeftDir = _CW;
      }else{
        motorLeftPwm = 0.0;
        motorLeftPwmFinal = 0.0;
        motorLeft->setmotor( motorLeftDir, 0);
        motorLeft->setmotor(_STOP);
      }


      // right motor
      if(motorRightPwm > deadZoneThreshold){
        motorRightDir = _CCW;
      }else if(motorRightPwm < -deadZoneThreshold){
        motorRightDir = _CW;
      }else{
        motorRightPwm = 0.0;
        motorRightPwmFinal = 0.0;
        motorRight->setmotor( motorRightDir, 0);
        motorRight->setmotor(_STOP);
      }

      motorLeft->setmotor( motorLeftDir, motorLeftPwmFinal);
      motorRight->setmotor( motorRightDir, motorRightPwmFinal);
    }
    if(throttleChanged){
      throttleChanged = false;
    }
    if(xChanged){
      xChanged = false;
    }
  }
      
}


// callback when data is recv from Master
void OnDataRecvMotor(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  memcpy(&commandData, data, sizeof(commandData));

  // Serial.printf(" CommandType=%c, Payload=%0.0f%%\n", commandData.commandType, commandData.payload); 
  switch(commandData.commandType){
    case 'x':
      x = ((float(commandData.payload))/512);
    break;
    case 'y':
      throttle = ((float(commandData.payload))/512);
    break;
  }
}