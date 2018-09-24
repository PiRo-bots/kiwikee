#include <ESP8266WiFi.h>

  String menuItems[4] = { 
    "1-ROULE",
    "2-AUTO",
    "3-GRAB",
    "4-DANSE"
  };
  int selectedItem = 0;
  char modes[4] = {
    'r', //rc
    'a', //auto
    'g', //grab
    'd', //dance
  };
  char mode = 'r';
  // r => rc
  // 'c' => config

#define NUM_MODES (unsigned int)(sizeof(menuItems)/sizeof(menuItems[0]))

#include "config.h"
#include "esprc.h"
#include "display.h"

unsigned long lastReading;

// for 4051 mux
#define MUX_A D8
#define MUX_B D7
#define MUX_C D6


int buttonPin = D3;
volatile int buttonState = 0;         // variable for reading the pushbutton status
volatile int prevButtonState = 0;

int xAxisPin = A0;
int xAxisValue = 0;
int prevXAxisValue = 0;

int yAxisPin = A0;
int yAxisValue = 0;
int prevYAxisValue = 0;

bool invertX = true;
bool invertY = true;

int xTrim = 29;
int yTrim = 15;

void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}

unsigned long lastButtonReading = millis();
unsigned long pressTime = 0;
unsigned long lastButtonInterrupt;
unsigned int longPressTime = 400;

void joystickButtonPressed(){
  if(millis() - lastButtonInterrupt > 10){ // prevents issues such as https://stackoverflow.com/questions/29756145/arduino-attachinterrupt-seems-to-run-twice
    prevButtonState = buttonState;
    buttonState = digitalRead(buttonPin);

    // 0 => down, 1 => up
    if (buttonState == 1) {
      // Serial.println("state");
      // Serial.println(buttonState);
      unsigned long currentTime = millis();
      pressTime  = currentTime - lastButtonReading;
      // Serial.println("pressTime");
      // Serial.println(pressTime);
      if(pressTime>longPressTime)
      {
        Serial.println("longPress");
        if(mode != 'c'){
          mode = 'c';
        }
        else{
          mode = 'r';
        }
      }else{
        Serial.println("shortPress");

        if(mode =='c'){
          Serial.println(menuItems[selectedItem]);
          mode = modes[selectedItem];
          sendSignalToSlave('m', selectedItem); // tell robot to switch modes
        }else{
          // trigger action
          sendSignalToSlave('a', 0); // tell robot to trigger action (mode specific)
        }
      }
    }
    if(buttonState ==0){
      lastButtonReading = millis();
    }
    lastButtonInterrupt = millis();
    drawMenu();
  }

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  /*if (prevButtonState != buttonState && buttonState != HIGH) {
    Serial.println("pressed");
    sendSignalToSlave('b', 1.0);
  }*/
}

//
typedef void (*GeneralMessageFunction) (char, float, float);

bool menuChangeDone = false;
void menuModeYAxisChanged (char command, float value, float prevValue){
  if(!menuChangeDone){
     if(prevValue>value && selectedItem + 1 < NUM_MODES){
      selectedItem ++ ;
    }else{
      if(selectedItem && selectedItem - 1 >= 0 ){
        selectedItem --;
      }
    }
    // menu stuff
    drawMenu();
    menuChangeDone = true;
  }
  if(abs(value) < 10 && prevValue != 0){
    menuChangeDone = false;
  }
}

void menuModeXAxisChanged (char command, float value, float prevValue){

}

void driveModeXAxisChanged (char command, float value, float prevValue){
  if(abs(prevValue-value)>2){
      // Serial.print("x:  ");
      // Serial.println(value);
      sendSignalToSlave('x', float(value));
  }
}

void driveModeYAxisChanged (char command, float value, float prevValue){
  if(abs(prevValue-value)>2){
    // Serial.print("y:  ");
    // Serial.println(value);
    sendSignalToSlave('y', float(value));
  }
}

// void checkPin (const int pin, GeneralMessageFunction response); // prototype

void readJoystickAxes(GeneralMessageFunction xChangeCallback, GeneralMessageFunction yChangeCallback){
  lastReading = millis ();

  prevXAxisValue = xAxisValue;
  changeMux(LOW, HIGH, LOW); // Value of the sensor connected Option 2 pin of Mux
  xAxisValue = analogRead(A0);
  
  if(invertX){
    xAxisValue = 1024 - xAxisValue;
  }
  // transform and ajust raw values
  xAxisValue = map(xAxisValue, 0, 1019, 0, 1024);
  xAxisValue = xAxisValue - 512;
  xAxisValue = xAxisValue + xTrim;
  xAxisValue = constrain(xAxisValue, -511, 511);

  if(abs(prevXAxisValue-xAxisValue)>2){
    xChangeCallback('x', xAxisValue, prevXAxisValue);
  }

  // now deal with Y axis
  prevYAxisValue = yAxisValue;
  changeMux(LOW, LOW, HIGH); // Value of the sensor connected Option 1 pin of Mux

  yAxisValue = analogRead(A0); 
  if(invertY){
    yAxisValue = 1024 - yAxisValue;
  }
  // transform and ajust raw values
  yAxisValue = map(yAxisValue, 0, 1019, 0, 1024);
  yAxisValue = yAxisValue - 512;
  yAxisValue = yAxisValue + yTrim;
  yAxisValue = constrain(yAxisValue, -511, 511);
  //
  if(abs(prevYAxisValue-yAxisValue)>2){
    yChangeCallback('y', yAxisValue, prevYAxisValue);
  }
}

void setupMultiplexer(){
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
}

void setup() {
  // pinMode(13, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(buttonPin, HIGH);

  pinMode(xAxisPin, INPUT_PULLUP);
  pinMode(yAxisPin, INPUT_PULLUP);

  setupMultiplexer();

  // setup interupts
  attachInterrupt(digitalPinToInterrupt(buttonPin), joystickButtonPressed, CHANGE);


	Serial.begin(115200);
	Serial.println("Initializing emitter");

  setupDisplay();
  setupEspNow();

  // draw menu on display
  drawMenu();
}

void loop() {
  // 5 => 200 hz
    if(mode =='c'){// configure
      if (millis () - lastReading >= 200) 
      {
        // menuChangeDone = false;
        readJoystickAxes(menuModeXAxisChanged, menuModeYAxisChanged);
      }
    }
    else if(mode == 'r'){
      if (millis () - lastReading >= 20)
      {
        readJoystickAxes(driveModeXAxisChanged, driveModeYAxisChanged);
      }
    }
    else if(mode == 'g'){
      if (millis () - lastReading >= 20)
      {
        readJoystickAxes(driveModeXAxisChanged, driveModeYAxisChanged);
      }
    }
}
