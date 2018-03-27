#include "SevSeg.h"
#include "SoftwareSerial.h"

SevSeg display7seg;

int pinBtn = A0;
int pinLock = A1;
int pinMotor = A2;
int pinBuzzer = A3;

boolean motorOn = false;
int seconds = 0;
int minutes = 0;

char tempString[10];

void setup()
{
  pinMode(pinMotor, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  stopCfa();
  Serial.begin(9600);
  
  // Common anode display
  int displayType = COMMON_ANODE; 
    
  //Digit pin definition
  int digit1 = 10; //first digit
  int digit2 = 11; //second digit
  int digit3 = 12; //third digit
  int digit4 = 13; //fourth digit

  int segA = 2; //segment A  
  int segB = 3; //segment B
  int segC = 4; //segment C
  int segD = 5; //segment D
  int segE = 6; //segment E
  int segF = 7; //segment F
  int segG = 8; //segment G
  int segDP= 9; //segment H
  
  int numberOfDigits = 4;

  //display initializing
  display7seg.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  
  //display brightness
  display7seg.SetBrightness(50);
}

void loop()
{
  if(isOpen()) {
    stopCfa();
    sprintf(tempString, "OPEN");
    display7seg.DisplayString(tempString, 0);
  } else {
    int val = analogRead(pinBtn);
  
    if(val > 0) {
      Serial.println(val);
      if(!motorOn) {
        if(val > 1000 && val < 1010) { // 220 ohm
          if(seconds == 0 && minutes > 0) {
            minutes--;
            seconds = 59;
          } else if(seconds > 0)
            seconds--;
        }
      
        if(val > 925 && val < 940) { // 1k ohm
          if(seconds == 59) {
            minutes++;
            seconds = 0;
          } else {
            seconds++;
          }
        }
      
        if(val > 90 && val < 95) { // 100k ohm
          if(seconds >= 10)
            seconds -= 10;
        }
      
        if(val > 690 && val < 700) { // 4.6k ohm
          seconds += 10;
          if(seconds > 59) {
            seconds = seconds - 60;
            minutes++;
          }
        }
    
        if(val > 965 && val < 975) { // 560 ohm
          if(minutes > 0)
            minutes--;
        }
    
        if(val > 1010 && val < 1014) {
            minutes++;
        }
    
        if(val > 1015 && val < 1020) {
          if(minutes >= 10)
            minutes -= 10;
        }
      }
    
      if(val > 1020) {
        //minutes += 10;
        turnOnOff();
      }
      
      while(analogRead(pinBtn) > 0) {
        delay(25);
      }
    }
    
    sprintf(tempString, "%02d%02d", minutes, seconds);
    display7seg.DisplayString(tempString, 2);
  }
  
}

void stopCfa() {
  analogWrite(pinMotor, 0);
  motorOn = false;
}

void startCfa() {
  unsigned int loops = seconds + (minutes * 60);
  Serial.print("Loops to run: ");
  Serial.println(loops);
  if(!isOpen()) {
    analogWrite(pinMotor, 255);
    motorOn = true;
    for(unsigned int x = 0; x < loops; x++) {
      if(seconds == 0) {
        if(minutes == 0)
          turnOnOff();
        else {
          minutes--;
          seconds = 59;
        }
      } else
        seconds--;

      setDisplay();
      Serial.print("Loop: ");
      Serial.println(x);
      if(!wait()) {
        break;
      }
    }

    if(seconds == 0 && minutes == 0 && motorOn) {
      turnOnOff();
      beep(3);
    }
  }
}

boolean wait() {
  delay(100);
  for(int x = 0; x <= 105; x++) {
    delay(1);
    setDisplay();
    
    if(isOpen()) {
      stopCfa();
      return false;
    }
    
    if(analogRead(pinBtn) >= 1020) {
      turnOnOff();
      return false;
    }
  }
  return true;
}

void turnOnOff() {
  if(motorOn)
    stopCfa();
  else
    startCfa();
}

void setDisplay() {
  sprintf(tempString, "%02d%02d", minutes, seconds);
  display7seg.DisplayString(tempString, 2);
}

boolean isOpen() {
  int lock = analogRead(pinLock);
  if(lock > 30)
    return true;
  else
    return false;
}

void beep(unsigned int times) {
  for(unsigned int x = 0; x < times; x++) {
    analogWrite(pinBuzzer, 255);
    delay(200);
    analogWrite(pinBuzzer, 0);
    delay(200);
  }
}