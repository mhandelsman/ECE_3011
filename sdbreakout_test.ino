#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Pin Definitions
#define SD_CS 6               // SD card chip select pin
#define SPI_MISO 19           // SPI MISO (DO)
#define SPI_MOSI 23           // SPI MOSI (DI)
#define SPI_SCK 18            // SPI Clock (SCK)
#define AUDIO_PIN 13         // PWM audio output pin

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  // Set up AUDIO_PIN as output
  pinMode(AUDIO_PIN, OUTPUT);

  // Initialize SPI for the SD card
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  // Initialize the SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.println("SD card initialized successfully.");
}

void loop() {
  // Play a WAV file from SD card
  playWavFile("/ledone.wav");  // Replace with your WAV file name
  delay(5000);  // Optional: Delay between playback
}

// Function to play audio using PWM from a WAV file
void playWavFile(const char* filename) {
  File audioFile = SD.open(filename);
  if (!audioFile) {
    Serial.println("Failed to open audio file");
    return;
  }

  Serial.print("Playing file: ");
  Serial.println(filename);

  // Read and output audio samples (low-quality simulation with PWM)
  while (audioFile.available()) {
    uint8_t sample = audioFile.read();   // Read one byte (sample)
    analogWrite(AUDIO_PIN, sample);      // Output the audio sample to the PWM pin
    delayMicroseconds(125);              // Adjust delay to simulate audio sampling rate
  }

  audioFile.close();
  Serial.println("Playback finished");
}
