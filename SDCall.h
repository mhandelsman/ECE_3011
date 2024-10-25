#ifndef SDCALL_H
#define SDCALL_H

#include <Arduino.h>
#include "SPI.h"
#include "SD.h"

class SDCall {
  private:
    int csPin;  // Chip Select pin for SD card

  public:
    // Constructor: Initializes the SDCall object with a CS pin
    SDCall(int csPin);

    // Method to initialize the SD card
    bool initSD();
};

#endif
