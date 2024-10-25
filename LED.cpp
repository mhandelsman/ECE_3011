#include <Arduino.h>
#include "led.h"

LED ledOne(10);  // LED connected to pin 10
LED ledTwo(11);  // LED connected to pin 11
LED ledThree(12);  // LED connected to pin 12

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Blink LED One for 500ms
  ledOne.blink(500);

  // Toggle LED Two
  ledTwo.toggle();
  delay(1000);  // Wait for 1 second

  // Turn on LED Three
  ledThree.turnOn();
  delay(2000);  // Leave it on for 2 seconds
  ledThree.turnOff();
}
