#include <Arduino.h>

void set_colour(const uint8_t r, const uint8_t g, const uint8_t b);

const int8_t sens = A0;
const int8_t red = 3;
const int8_t green = 6;
const int8_t blue = 9;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void loop() {

  set_colour(0, 0, 0);

  if (analogRead(sens) < 5) {

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