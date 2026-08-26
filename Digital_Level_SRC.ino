#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "GY521.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

//calibrate button to current position
const int calibrate = 34;
//"zero" the values back to earth's gravity
const int zero = 35;



const int upLED = 26;
const int downLED = 14;
const int leftLED = 12;
const int rightLED = 27;



Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

GY521 mpu(0x69);

void setup() {
  Serial.begin(115200);

  pinMode(calibrate, INPUT);
  pinMode(zero, INPUT);

  pinMode(upLED, OUTPUT);
  pinMode(downLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(leftLED, OUTPUT);
  
  Wire.begin(21, 22);

  if (!mpu.begin()) {
    Serial.println("MPU-6050 not found!");
    while (1);
  }
  

  


  Serial.println("MPU-6050 connected!");
  //gyro sensitivity
  if (mpu.setGyroSensitivity(1)) {
    Serial.println("Sensitivity set to 500 degrees successfully!");
  } else {
    Serial.println("Failed to set sensitivity!");
  }
  mpu.calibrate(100, 0 , 0, false);


  
  
  

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    while (true);
  }
  // put your setup code here, to run once:

}


bool previousCali = LOW;
bool previousZero = LOW;

float expoMovAvg(float reading, float previousReading, float alpha){
  return alpha * (reading) + ((1.0 -alpha)*previousReading);
}


float previousY = 0;
float previousX = 0;
float previousZ = 0;

float previousXIntv = 0;
float previousYIntv = 0;

bool upLEDState = LOW;
bool downLEDState = LOW;
bool leftLEDState = LOW;
bool rightLEDState = LOW;

float setX = 0;
float setY = 0;

void loop() {


  mpu.read();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  float X = expoMovAvg(mpu.getAccelX(), previousX, 0.05);
  previousX = X;
  float displayX = X + setX;
  
  display.print("x: ");
  display.println(displayX);

  if(displayX> 0.025){
    float dlay = 1000.0*displayX;
    float intv1 = millis();
    if(previousXIntv < intv1){
      if(upLEDState){
        digitalWrite(upLED, LOW);
        upLEDState = LOW;
      }
      else{
        digitalWrite(upLED,HIGH);
        upLEDState = HIGH;
      }
      previousXIntv = intv1 + dlay;
    }
  }
  else{
    digitalWrite(upLED,HIGH);
  }

  if(displayX< -0.025){
    float dlay = 1000.0*abs(displayX);
    float intv1 = millis();
    if(previousXIntv < intv1){
      if(downLEDState){
        digitalWrite(downLED, LOW);
        downLEDState = LOW;
      }
      else{
        digitalWrite(downLED,HIGH);
        downLEDState = HIGH;
      }
      previousXIntv = intv1 + dlay;
    }
  }
  else{
    digitalWrite(downLED,HIGH);
  }
  
  

  float Y = expoMovAvg(mpu.getAccelY(), previousY, 0.05);
  previousY = Y;
  float displayY = Y + setY;

  display.print("y: ");
  display.println(displayY);
  if(displayY> 0.025){
    float dlay = 1000.0*displayY;
    float intv2 = millis();
    if(previousYIntv < intv2){
      if(leftLEDState){
        digitalWrite(leftLED, LOW);
        leftLEDState = LOW;
      }
      else{
        digitalWrite(leftLED,HIGH);
        leftLEDState = HIGH;
      }
      previousYIntv = intv2 + dlay;
    }
  }
  else{
    digitalWrite(leftLED,HIGH);
  }

  if(displayY< -0.025){
    float dlay = 1000.0*abs(displayY);
    float intv2 = millis();
    if(previousYIntv < intv2){
      if(rightLEDState){
        digitalWrite(rightLED, LOW);
        rightLEDState = LOW;
      }
      else{
        digitalWrite(rightLED,HIGH);
        rightLEDState = HIGH;
      }
      previousYIntv = intv2 + dlay;
    }
  }
  else{
    digitalWrite(rightLED,HIGH);
  }

  float Z = expoMovAvg(mpu.getAccelZ(), previousZ, 0.05);
  previousZ = Z;

  display.print("z: ");
  display.println(Z);



  bool currentCali = digitalRead(calibrate);
  bool currentZero = digitalRead(zero);

  //cali = left button
  //zero = right button


  if(currentCali == HIGH && previousCali == LOW){
    setX = -X;
    setY= -Y;
    Serial.println("left button pressed");
  }
  previousCali = currentCali;


  //"zero" the values to whatever position I am currently at
  if(currentZero == HIGH && previousZero == LOW){
    setX = 0;
    setY = 0;
    Serial.println("right button pressed");
  }

  previousZero = currentZero;

  display.println();
  display.display();
  delay(1);
    
  // put your main code here, to run repeatedly:

}
