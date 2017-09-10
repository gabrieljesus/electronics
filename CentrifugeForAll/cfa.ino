#include <everytime.h>

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Analog input for buttons
int button = A0;

// Motor
int motor = 7;

int hours;
int minutes;
int seconds;
int hoursLimit = 3;

int vHour = 20;
int vMin = 150;
int vSec = 250;
int vOnOff = 500;

boolean on = false;

char *formated_hours = malloc(3);
char *formated_minutes = malloc(3);
char *formated_seconds = malloc(3);

void setup() {
  Serial.begin(9600);
  // Set up the button as input
  pinMode(button, INPUT);
  // Set up motor as output
  pinMode(motor, OUTPUT);
  
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
  clearAll();
}

void clearAll() {
  lcd.clear();

  digitalWrite(motor, LOW);
  
  hours = 0;
  minutes = 0;
  seconds = 0;

  sprintf(formated_hours, "%02d", hours);
  sprintf(formated_minutes, "%02d", minutes);
  sprintf(formated_seconds, "%02d", seconds);
  
  lcd.print("Cycle time");
  lcd.setCursor(0, 1);
  lcd.print(formated_hours);
  lcd.print(":");
  lcd.print(formated_minutes);
  lcd.print(":");
  lcd.print(formated_seconds);

  lcd.setCursor(9, 1);
  lcd.print("OFF");
}

void loop() {
  int buttonState = analogRead(button);
  
  if(buttonState > 0) {
    Serial.println(buttonState);
    if(!on) {
      if(buttonState < vHour) {
        hours++;
        if(hours > hoursLimit)
          hours = 0;
        setHour();
      } else if(buttonState < vMin) {
        minutes++;
        if(minutes > 59)
          minutes = 0;
        setMinute();
      } else if(buttonState < vSec){
        seconds++;
        if(seconds > 59)
          seconds = 0;
        setSecond();
      }
    }
    
    if(buttonState >= vOnOff) {
      if(hours != 0 || minutes != 0 || seconds != 0) {
        turnOnOff();
        unsigned int loops = getLoops();
        for(unsigned int x = 0; x < loops; x++) {
          if(seconds == 0) {
            if(minutes == 0) {
              if(hours == 0) {
                turnOnOff();
              } else {
                hours--;
                seconds = 59;
                minutes = 59;
                setHour();
                setMinute();
                setSecond();
              }
            } else {
              minutes--;
              seconds = 59;
              setMinute();
              setSecond();
            }
          } else {
            seconds--;
            setSecond();
          }

          if(!wait()) {
            break;
          }
        }
      }

      if(seconds == 0 && minutes == 0 && hours == 0 && on)
        turnOnOff();
    }

    while(analogRead(button) > 0) {
      delay(50);
    }
  }
}

void turnOnOff() {
  lcd.setCursor(9, 1);
  if(on) {
    lcd.print("OFF");
    digitalWrite(motor, LOW);
    on = false;
  } else {
    lcd.print("ON ");
    digitalWrite(motor, HIGH);
    on = true;
  }
}

void setHour() {
  sprintf(formated_hours, "%02d", hours);
  lcd.setCursor(0, 1);
  lcd.print(formated_hours);
}

void setMinute() {
  sprintf(formated_minutes, "%02d", minutes);
  lcd.setCursor(3, 1);
  lcd.print(formated_minutes);
}

void setSecond() {
  sprintf(formated_seconds, "%02d", seconds);
  lcd.setCursor(6, 1);
  lcd.print(formated_seconds);
}

unsigned int getLoops() {
  return seconds + (minutes * 60) + (hours * 3600);
}

boolean wait() {
  delay(250);
  for(int x = 0; x < 15; x++) {
    delay(50);
    if(analogRead(button) >= vOnOff) {
      turnOnOff();
      return false;
    }
  }
  return true;
}

