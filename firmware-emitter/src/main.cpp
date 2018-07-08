/*#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
	#include <espnow.h>
	#include <user_interface.h>
}*/

#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
}

#include "espnow_setup.h"

unsigned long lastReading;


// for 4051 mux
#define MUX_A D4
#define MUX_B D3
#define MUX_C D2


int buttonPin = D1;
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
    // Serial.println("pressed");
    sendSignalToSlave('b', 1.0);
  }
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(buttonPin, HIGH);

  pinMode(xAxisPin, INPUT_PULLUP);
  pinMode(yAxisPin, INPUT_PULLUP);

  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);


  // setup interupts
  attachInterrupt(digitalPinToInterrupt(buttonPin), joystickButtonPressed, CHANGE);


	Serial.begin(115200);
	Serial.println("Initializing...");

  setupEspNow();
}



void loop() {
  if (millis () - lastReading >= 5)   // 200 Hz
  {
    lastReading = millis ();

    prevXAxisValue = xAxisValue;
    changeMux(LOW, LOW, HIGH);//Value of the sensor connected Option 1 pin of Mux
    xAxisValue = analogRead(A0); 
    //xAxisValue = analogRead(xAxisPin);
    
    if(abs(prevXAxisValue-xAxisValue)>2){
        Serial.print("x:  ");
        Serial.println(xAxisValue);
        sendSignalToSlave('x', float(xAxisValue));
    }

    prevYAxisValue = yAxisValue;
    changeMux(LOW, HIGH, LOW);//Value of the sensor connected Option 2 pin of Mux
    yAxisValue = analogRead(A0); 
    //yAxisValue = analogRead(yAxisPin);
    
    if(abs(prevYAxisValue-yAxisValue)>2){
      Serial.print("y:  ");
      Serial.println(yAxisValue);
      sendSignalToSlave('y', float(yAxisValue));
    }
  }
   
}
