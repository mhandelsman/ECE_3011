#include "LED.h"

// Constructor to initialize the pin
LED::LED(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);  // Set pin as output
}

// Turn on the LED
void LED::turnOn() {
  digitalWrite(_pin, HIGH);
}

// Turn off the LED
void LED::turnOff() {
  digitalWrite(_pin, LOW);
}

// Blink the LED with a delay time
void LED::blink(int delayTime) {
  turnOn();               // Turn on LED
  delay(delayTime);        // Wait for delayTime
  turnOff();              // Turn off LED
  delay(delayTime);        // Wait for delayTime
}
