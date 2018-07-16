#include <ESP8266WiFi.h>

  String menuItems[3] = { 
    "1-ROULE",
    "2-DANCE",
    "3-GROGNE"
  };
  int selectedItem = 0;
  char mode = 'd'; // d => drive
  // 'c' => config

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


void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}

void joystickButtonPressed(){
  prevButtonState = buttonState;
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (prevButtonState != buttonState && buttonState != HIGH) {
    Serial.println("pressed");
    sendSignalToSlave('b', 1.0);
  }
}

//
typedef void (*GeneralMessageFunction) (char, float, float);

void menuModeYAxisChanged (char command, float value, float prevValue){
  if(prevValue>value){
        selectedItem -- ;
    }else{
      selectedItem ++;
    }
    // menu stuff
    if(selectedItem >2){
      selectedItem = 0;
    }
    if(selectedItem <0){
      selectedItem = 2;
    }
    drawMenu();
}

void menuModeXAxisChanged (char command, float value, float prevValue){

}

void driveModeXAxisChanged (char command, float value, float prevValue){
  if(abs(prevValue-value)>2){
      Serial.print("x:  ");
      Serial.println(value);
      sendSignalToSlave('x', float(value));
  }
}

void driveModeYAxisChanged (char command, float value, float prevValue){
  if(abs(prevValue-value)>2){
    Serial.print("y:  ");
    Serial.println(value);
    sendSignalToSlave('y', float(value));
  }
}

// void checkPin (const int pin, GeneralMessageFunction response); // prototype

void readJoystickAxes(GeneralMessageFunction xChangeCallback, GeneralMessageFunction yChangeCallback){
  lastReading = millis ();

  prevXAxisValue = xAxisValue;
  changeMux(LOW, LOW, HIGH); // Value of the sensor connected Option 1 pin of Mux
  xAxisValue = analogRead(A0); 
  
  if(abs(prevXAxisValue-xAxisValue)>2){
    xChangeCallback('x', xAxisValue, prevXAxisValue);
  }

  prevYAxisValue = yAxisValue;
  changeMux(LOW, HIGH, LOW); // Value of the sensor connected Option 2 pin of Mux
  yAxisValue = analogRead(A0); 
  
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
  if (millis () - lastReading >= 5) // 200 Hz
  {
    // readJoystickAxes(menuModeXAxisChanged, menuModeYAxisChanged);
    readJoystickAxes(driveModeXAxisChanged, driveModeYAxisChanged);
  }
   
}
