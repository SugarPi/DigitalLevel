#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "GY521.h"
#include "esp_sleep.h"
#include "driver/gpio.h"



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C



//on/off button
const int onOff = 0;
//"zero" the values to current angle
const int zero = 10;



const int upLED = 4;
const int downLED = 3;
const int leftLED = 1;
const int rightLED = 2;


Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

GY521 mpu(0x69);

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);

  pinMode(onOff, INPUT);
  pinMode(zero, INPUT);

  pinMode(upLED, OUTPUT);
  pinMode(downLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(leftLED, OUTPUT);

  /*
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO) {
  // Wait for user to let go of the button after waking the chip
  while (digitalRead(onOff) == HIGH) {
    delay(10);
  }
  delay(100);
  }
  */
  if (!mpu.begin()) {
    Serial.println("MPU-6050 not found!");
    while (1);
  }
  mpu.setGyroSensitivity(1);
  mpu.calibrate(100, 0 , 0, false);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  
}

//boolean variable to record button states to make sure buttons only register once even while held down
bool previousZero = LOW;




//function to calculate exponential moving average
float expoMovAvg(float reading, float previousReading, float alpha){
  return alpha * (reading) + ((1.0 -alpha)*previousReading);
}

//function to t

void sleep() {
    esp_deep_sleep_enable_gpio_wakeup(1ULL << onOff,ESP_GPIO_WAKEUP_GPIO_HIGH);
    esp_deep_sleep_start();
}



//start of variables defined for dynamic led blinking interval depending on how big 
//the disparity between the set angle and the measured on
float previousY = 0;
float previousX = 0;
float previousZ = 0;

float previousXIntv = 0;
float previousYIntv = 0;


bool upLEDState = LOW;
bool downLEDState = LOW;
bool leftLEDState = LOW;
bool rightLEDState = LOW;
//end 

//variables set for "setting" the angles and remembering them
float setX = 0;
float setY = 0;

//this previous variable keeps track of often we should update the screen
//basic logic: if current time (millis) is greater than the previous one by 80 ms, we update the screen
//and set previous to the current time. Otherwise, we don't update anything
float previous = 0;


void loop() {
  
  mpu.read();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  //get raw readings of accelerometer and convert it into pitch and row
  float measuredRollAcc = (atan2(mpu.getAccelY(), mpu.getAccelZ()) *180.0) / 3.1415926;
  float measuredPitchAcc = (atan2(mpu.getAccelX(), mpu.getAccelZ()) *180.0) / 3.1415926;

  float X = expoMovAvg(mpu.getAccelX(), previousX, 0.05);
  previousX = X;
  float displayX = X + setX;
  //start: logic for the 4 leds
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


  //variable to keep ensure buttons are only registered once, even while held down
  bool currentonOff = digitalRead(onOff);
  bool currentZero = digitalRead(zero);

  //start of logic for on/off, only turns off once user holds for 4 seconds
  
  //records the time since first click
  unsigned long startClick = millis();

  while(digitalRead(onOff) == HIGH){
    if(millis() - startClick > 4000){

      //shut off leds and oled to signal user to lift their finger
      display.clearDisplay();
      display.display();

      digitalWrite(upLED, LOW);
      digitalWrite(downLED, LOW);
      digitalWrite(rightLED, LOW);
      digitalWrite(leftLED, LOW);
      //waits 2 seconds to make sure user lifts their finger to actually go to sleep
      delay(2000);
      sleep();
    }
    

  }
  

  


  //"zero" the values to whatever position I am currently at
  if(currentZero == HIGH && previousZero == LOW){
    setX = -X;
    setY= -Y;
    Serial.println("left button pressed");
  }

  previousZero = currentZero;

  //update screen every 0.3 seconds
  float current = millis();
  

  if(current - previous > 80){
    char roll[30];
    sprintf(roll, "Roll: %4.1f", abs(measuredRollAcc));
    display.println(roll);

    char pitch[30];
    sprintf(pitch, "Pitch:%4.1f", abs(measuredPitchAcc));
    display.println(pitch);
    previous = current;
    display.display();
    Serial.println(previous);
  }
  

  display.println();
  // put your main code here, to run repeatedly:
}
