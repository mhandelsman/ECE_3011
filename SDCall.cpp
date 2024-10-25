#include "sdcall.h"

// Constructor: Initializes the SDCard object
SDCall::SDCall(int csPin) {
  this->csPin = csPin;
}

// Method to initialize the SD card
bool SDCall::initSD() {
  if (!SD.begin(csPin)) {
    Serial.println("SD card initialization failed!");
    return false;
  }
  Serial.println("SD card initialized successfully.");
  return true;
}
