#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
  private:
    int buttonPin;
  
  public:
    // Constructor: Initializes the button object with a pin number
    Button(int pin);

    // Method to check if the button is pressed (LOW state)
    bool isPressed();
};

#endif
