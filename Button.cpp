#include "button.h"

// Constructor: Initializes the button pin
Button::Button(int pin) {
  buttonPin = pin;
  pinMode(buttonPin, INPUT_PULLUP);  // Button is active LOW, use internal pullup resistor
}

// Method to check if the button is pressed (LOW state)
bool Button::isPressed() {
  return digitalRead(buttonPin) == LOW;
}
