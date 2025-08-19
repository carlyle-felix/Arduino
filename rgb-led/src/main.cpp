#include <Arduino.h>

void set_colour(const uint8_t r, const uint8_t g, const uint8_t b);

const int8_t led = 13;
const int8_t red = 3;
const int8_t green = 6;
const int8_t blue = 9;
const int8_t button = 2;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  set_colour(0, 0, 0);
  digitalWrite(led, HIGH);
  while (digitalRead(button)) {
    digitalWrite(led, LOW);
    set_colour(255, 20, 147);
    delay(1000);
    set_colour(173, 216, 230);
    delay(1000);
    set_colour(128, 0, 128);
    delay(1000);
  }
}

void set_colour(const uint8_t r, const uint8_t g, const uint8_t b)
{
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);
}