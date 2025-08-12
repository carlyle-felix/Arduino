#include <Arduino.h>

#define NUM_FLASHES 3
#define STAGE_TIME 2000
#define MAX_REACT 3000
#define RAND_DELAY_MIN 500
#define RAND_DELAY_MAX 700

void set_tree_state(uint8_t state);
void reset_tree(void);
void start_tree(uint16_t ms);

const uint8_t sensor = 7;   // IR OBSTACLE AVOIDANCE SENSOR (SUNFOUNDER ST0050).
const uint8_t s = 5;        // YELLOW LED.
const uint8_t y = 4;        // YELLOW LED.
const uint8_t g = 3;        // GREEN LED.
const uint8_t r = 2;        // RED LED.

void setup(void) 
{
  pinMode(sensor, INPUT);
  pinMode(s, OUTPUT);
  pinMode(y, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);

  Serial.begin(9600);
}

void loop(void) 
{
  uint16_t ms;

  set_tree_state(LOW);

  srand(analogRead(A0));
  ms = random(RAND_DELAY_MIN, RAND_DELAY_MAX);

  if (!digitalRead(sensor)) {
    start_tree(ms);
    reset_tree();
  } 
}

void set_tree_state(uint8_t state)
{
  digitalWrite(s, state);
  digitalWrite(y, state);
  digitalWrite(g, state);
  digitalWrite(r, state);
}

void reset_tree(void) 
{
  register uint8_t i;

  // flash tree NUM_FLASHES times then turn off.
  for(i = 0; i < NUM_FLASHES; i++) {
    set_tree_state(LOW);
    delay(250);
    set_tree_state(HIGH);
    delay(250);
  }
  set_tree_state(LOW);
}

void start_tree(uint16_t ms)
{
  uint32_t start_time = millis(), leave_time;
  int32_t react;
  
  /*
      turn led_s on while sensor is covered and start sequence if sensor
      remains covered for STAGE_TIME milliseconds.
  */
  while (millis() - start_time < STAGE_TIME) {
    digitalWrite(s, LOW);
    if (!digitalRead(sensor)) {
      digitalWrite(s, HIGH);
      continue;
    }
    start_time = millis();
  }
  
  delay(500);
  digitalWrite(y, HIGH);

  /*
      after a delay set by caller, turn off led_s and led_y then check if 
      obstacle is still present then turn on led_g, else turn on led_r.
  */
  delay(ms);
  digitalWrite(s, LOW);
  digitalWrite(y, LOW);
  if (digitalRead(sensor)) {
    digitalWrite(r, HIGH);
    react = -1;
  } else {
    digitalWrite(g, HIGH);
    // mesaure reaction time in ms up until MAX_REACT.
    start_time = millis();
    while (millis() - start_time < MAX_REACT) {
      if (digitalRead(sensor)) {
        break;
      }
    }
    leave_time = millis();
    react = leave_time - start_time;
  }  

  // print reaction to serial.
  Serial.print("\nReaction time: ");
  if (react >= 0 && react < MAX_REACT) {
    Serial.print(react);
    Serial.println(" ms.");
  } else if (react >= MAX_REACT) {
    Serial.println("TIME-OUT");
  } else {
    Serial.println("JUMP");
  }

  delay(3000);
}