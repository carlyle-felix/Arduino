#include <Arduino.h>

#define ON HIGH
#define OFF LOW
#define NUM_FLASHES 3
#define STAGE_TIME 2000
#define MAX_REACT 3000

void set_tree_state(uint8_t state);
void reset_tree(void);
uint8_t start_tree(uint16_t ms);

const uint8_t sensor = 7;
const uint8_t s = 5;
const uint8_t y = 4;
const uint8_t g = 3;
const uint8_t r = 2;

void setup() 
{
  pinMode(sensor, INPUT);
  pinMode(s, OUTPUT);
  pinMode(y, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);

  Serial.begin(9600);
}

void loop() 
{
  uint8_t sensor_state, result;
  uint16_t ms;

  set_tree_state(OFF);

  srand(analogRead(A0));
  ms = random(500, 700);

  sensor_state = digitalRead(sensor);
  if (!sensor_state) {
    start_tree(ms);
    if (result) {
      set_tree_state(OFF);
    }
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

  for(i = 0; i < NUM_FLASHES; i++) {
    set_tree_state(OFF);
    delay(250);
    set_tree_state(ON);
    delay(250);
  }
}

uint8_t start_tree(uint16_t ms)
{
  uint32_t start_time = millis(), leave_time;
  uint16_t react;

  while (millis() - start_time < STAGE_TIME) {
    digitalWrite(s, OFF);
    if (!digitalRead(sensor)) {
      digitalWrite(s, ON);
      continue;
    }
    start_time = millis();
  }
  
  delay(500);
  digitalWrite(y, ON);
  delay(ms);
  start_time = millis();

  digitalWrite(s, OFF);
  digitalWrite(y, OFF);
  if (digitalRead(sensor)) {
    leave_time = millis();
    digitalWrite(r, ON);
  } else {
    digitalWrite(g, ON);
    while (millis() - start_time < MAX_REACT) {
      if (digitalRead(sensor)) {
        break;
      }
    }
    leave_time = millis();
  }  

  react = leave_time - start_time;
  Serial.print("\nReaction time: ");
  if (react > 1 && react < MAX_REACT) {
    Serial.print(react);
    Serial.println(" ms.");
  } else if (react >= MAX_REACT) {
    Serial.println("TIME-OUT");
  } else {
    Serial.println("JUMP");
  }

  delay(3000);

  return 0;
}