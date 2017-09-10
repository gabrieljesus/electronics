// This simulate a traffic light with 4 ways,
// but we can add more as needed

// Define pins
#define red1 8
#define yellow1 12
#define green1 13

#define red2 5
#define yellow2 6
#define green2 7

#define red3 11
#define yellow3 10
#define green3 9

#define red4 4
#define yellow4 3
#define green4 2

// Define times
#define green_to_yellow 5000
#define yellow_to_red 2500

void setup() {
  // Configure all pins with all lights
  pinMode(red1, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(green1, OUTPUT);
  
  pinMode(red2, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(green2, OUTPUT);
  
  pinMode(red3, OUTPUT);
  pinMode(yellow3, OUTPUT);
  pinMode(green3, OUTPUT);
  
  pinMode(red4, OUTPUT);
  pinMode(yellow4, OUTPUT);
  pinMode(green4, OUTPUT);
  
  // Turn off all lights
  digitalWrite(red1, LOW);
  digitalWrite(yellow1, LOW);
  digitalWrite(green1, LOW);
  
  digitalWrite(red2, LOW);
  digitalWrite(yellow2, LOW);
  digitalWrite(green2, LOW);
  
  digitalWrite(red3, LOW);
  digitalWrite(yellow3, LOW);
  digitalWrite(green3, LOW);
  
  digitalWrite(red4, LOW);
  digitalWrite(yellow4, LOW);
  digitalWrite(green4, LOW);
  
  // Set the initial state
  digitalWrite(green1, HIGH);
  digitalWrite(red2, HIGH);
  digitalWrite(red3, HIGH);
  digitalWrite(red4, HIGH);
}

void loop() {
// This is the basic workflow:
// Turn on green1, red2, red3 and red4 (and others if present)
// Turn off green1 and turn on yellow1
// Turn off yellow1, turn on red1, turn off red2 and turn on green2
// Turn off green2 and turn on yellow2
// Turn off yellow2, turn on red2, turn off red3 and turn on green3
// and so on...
  
  delay(green_to_yellow);
  
  changeState(green1, yellow1, red1, red2, green2);
  
  delay(green_to_yellow);
  
  changeState(green2, yellow2, red2, red3, green3);
  
  delay(green_to_yellow);
  
  changeState(green3, yellow3, red3, red4, green4);
  
  delay(green_to_yellow);
  
  changeState(green4, yellow4, red4, red1, green1);
  
}

void changeState(int green_old, int yellow_old, int red_old, int red_new, int green_new) {
  digitalWrite(green_old, LOW);
  digitalWrite(yellow_old, HIGH);
  
  delay(yellow_to_red);
  
  digitalWrite(yellow_old, LOW);
  digitalWrite(red_old, HIGH);
  digitalWrite(red_new, LOW);
  digitalWrite(green_new, HIGH);
}