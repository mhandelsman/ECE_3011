#ifndef LED_h
#define LED_h

#include <Arduino.h>

class LED {
  public:
    LED(int pin);   // Constructor to initialize the LED pin
    void turnOn();  // Function to turn on the LED
    void turnOff(); // Function to turn off the LED
    void blink(int delayTime); // Function to blink the LED with a delay
  private:
    int _pin;       // Pin associated with the LED
};

#endif
