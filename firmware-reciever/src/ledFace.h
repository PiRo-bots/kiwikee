#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
unsigned long lastReadingLed;

void setupFace() {
  matrix.begin(0x71);  // pass in the address
  matrix.clear();
}

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  etonne[] = {
      B11000011,
  B11000011,
  B00000000,
  B00000000,
  B00000000,
  B01111000,
  B01001000,
  B01111000
  },
  blague[] = {
 B11000011,
  B11000011,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11111111
  },
  zombie[] = {
      B11000011,
  B11000011,
  B00000000,
  B00000000,
  B00011000,
  B00011000,
  B01111110,
  B00000000
  },
  grogne[] = {
    B10000001,
  B01000010,
  B11000011,
  B11000011,
  B00000000,
  B11111111,
  B10000001,
  B11111111
  },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 };

const uint64_t IMAGES[] = {
  0x1866c38100e7a5e7
};
const int IMAGES_LEN = sizeof(IMAGES)/8;


int face = 0; // 1 => normal, 2 => too fast
int prevFace = 0;

void displayFace(float throttle, long distance) {
  if (millis () - lastReadingLed >= 300)
  {
    lastReadingLed = millis();
    if(fabs(throttle)> 0.2){
      face = 1;      
    }else{
      face = 0;
    }
    /* if(distance < 10){
      face = 2;
    }*/

    if(face != prevFace){
      matrix.clear();
      if(face == 1) {
        Serial.println("switching to scared");
        matrix.drawBitmap(0, 0, etonne, 8, 8, LED_ON);
        // matrix.drawBitmap(0, 0, zombie, 8, 8, LED_ON);
         
        // matrix.drawBitmap(0, 0, zombie, 8, 8, LED_ON);
        matrix.writeDisplay();

      }else if(face == 0){
        Serial.println("switching to normal");
        matrix.drawBitmap(0, 0, blague, 8, 8, LED_ON);
        //matrix.drawBitmap(0, 0, grogne, 8, 8, LED_ON);
        matrix.writeDisplay();
      } else if(face == 2) {
        Serial.println("switching to grogne");
        matrix.drawBitmap(0, 0, grogne, 8, 8, LED_ON);
        //matrix.drawBitmap(0, 0, grogne, 8, 8, LED_ON);
        matrix.writeDisplay();
      }

      //matrix.writeDisplay();
      prevFace = face;
      
    }
    /*matrix.clear();
  matrix.drawBitmap(0, 0, zombie, 8, 8, LED_ON);
  matrix.writeDisplay();
  // delay(1500);

  matrix.clear();
  matrix.drawBitmap(0, 0, grogne, 8, 8, LED_ON);
  matrix.writeDisplay();*/
  // delay(1500);
   
  }
  

  /*
  delay(1500);

    matrix.clear();
  matrix.drawBitmap(0, 0, zombie, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(1500);

    matrix.clear();
  matrix.drawBitmap(0, 0, grogne, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(1500);*/

  /*matrix.clear();
  matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);*/

  /*matrix.clear();      // clear display
  matrix.drawPixel(0, 0, LED_ON);  
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix.clear();
  matrix.drawLine(0,0, 7,7, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);*/
}