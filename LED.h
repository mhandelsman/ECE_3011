#include "led.h"

// Constructor: Initializes the LED pin
LED::LED(int pin) {
  ledPin = pin;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Ensure LED is off by default
}

// Method to turn the LED on
void LED::turnOn() {
  digitalWrite(ledPin, HIGH);
}

// Method to turn the LED off
void LED::turnOff() {
  digitalWrite(ledPin, LOW);
}

// Method to toggle the LED (turn on if off, or off if on)
void LED::toggle() {
  int state = digitalRead(ledPin);
  digitalWrite(ledPin, !state);
}

// Method to blink the LED for a given duration
void LED::blink(int duration) {
  turnOn();
  delay(duration);
  turnOff();
}
