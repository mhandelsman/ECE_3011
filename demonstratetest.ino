#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"

// Pin definitions for the SD card and MAX98357
#define SD_CS 18      // Chip select for SD card
#define SPI_MISO 20   // SPI MISO (DO)
#define SPI_MOSI 19   // SPI MOSI (DI)
#define SPI_SCK 21    // SPI Clock (SCK)

// Pin definitions for LEDs
#define ledone 10       // LED for button 1
#define ledtwo 11       // LED for button 2
#define ledthree 12     // LED for button 3

// Pin definitions for buttons
#define button1Pin 4
#define button2Pin 5
#define button3Pin 15
#define button4Pin 8   // Button 4 defined but not used

// Pin definitions for MAX98357 I2S interface
#define I2S_DOUT 13   // DIN pin of the MAX98357 (Audio Data)
#define I2S_BCLK 3    // BCLK pin of the MAX98357 (Bit Clock)
#define I2S_LRC 2     // LRC pin of the MAX98357 (Word Select/Left Right Clock)
#define I2S_SD 9      // SD pin of the MAX98357 (used to enable/disable the amp, tied to GPIO9 or 3.3V)
#define GAIN_PIN 23   // Pin to control gain

AudioGeneratorWAV *wav;            // WAV generator
AudioFileSourceSD *file;           // SD card file source
AudioOutputI2S *out;               // I2S output to MAX98357
bool isPlaying = false;            // Flag to prevent multiple files from playing at once
int activeLed = -1;                // Tracks which LED is active during playback

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  // Initialize LED pins
  pinMode(ledone, OUTPUT);
  pinMode(ledtwo, OUTPUT);
  pinMode(ledthree, OUTPUT);

  // Turn off all LEDs at the start
  digitalWrite(ledone, LOW);
  digitalWrite(ledtwo, LOW);
  digitalWrite(ledthree, LOW);

  // Initialize button pins
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);  // Button 4 is defined but not used

  // Initialize SPI for SD card
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  Serial.println("SD card initialized successfully.");

  // Set up GAIN pin for the MAX98357
  pinMode(GAIN_PIN, OUTPUT);
  digitalWrite(GAIN_PIN, HIGH);  // Set gain to high (9dB). Set LOW for lower gain (3dB).

  // Set up I2S for MAX98357
  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT); // Set I2S pins
  out->SetGain(1.0);  // Set maximum volume (1.0)

  // Prepare the WAV generator but don't open any file yet
  wav = new AudioGeneratorWAV();
}

void playWavFile(const char* filename, int ledPin) {
  if (isPlaying) return;  // If something is playing, don't start a new file

  file = new AudioFileSourceSD(filename);  // OPEN THE FILE
  if (!file->isOpen()) {
    Serial.println("Error: Could not open WAV file!");
    return;
  }

  wav->begin(file, out);
  Serial.print("Playing: ");
  Serial.println(filename);
  digitalWrite(ledPin, HIGH);  // Turn on the LED for this button
  isPlaying = true;
  activeLed = ledPin;  // Set active LED to the one associated with the playing file
}

void stopPlayback() {
  if (wav->isRunning()) {
    wav->loop();  // Continue looping the current file
  } else {
    wav->stop();
    file->close();
    Serial.println("Playback finished.");
    digitalWrite(activeLed, LOW);  // Turn off the LED that was active during playback
    isPlaying = false;
    activeLed = -1;  // Reset active LED
  }
}

void loop() {
  // If audio is playing, handle playback without checking buttons
  if (isPlaying) {
    stopPlayback();  // Only check the audio status and loop
    return;
  }

  // If no audio is playing, check for button presses
  if (!isPlaying) {
    // If button 1 is pressed and no audio is playing, play ledone.wav
    if (digitalRead(button1Pin) == LOW) {
      playWavFile("/ledone.wav", ledone);
    }

    // If button 2 is pressed and no audio is playing, play ledtwo.wav
    else if (digitalRead(button2Pin) == LOW) {
      playWavFile("/ledtwo.wav", ledtwo);
    }

    // If button 3 is pressed and no audio is playing, play ledthree.wav
    else if (digitalRead(button3Pin) == LOW) {
      playWavFile("/ledthree.wav", ledthree);
    }
  }

  delay(100);  // Debounce delay
}
