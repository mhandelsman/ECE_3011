#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"

// Pin definitions for the SD card and MAX98357
#define SD_CS 18
#define SPI_MISO 20
#define SPI_MOSI 19
#define SPI_SCK 21

// Pin definitions for LEDs
#define ledone 10
#define ledtwo 11
#define ledthree 12

// Pin definitions for buttons
#define button1Pin 4
#define button2Pin 5
#define button3Pin 15
#define button4Pin 8   // Unused, could be used for a reset or additional interaction

// Pin definitions for MAX98357 I2S interface
#define I2S_DOUT 13
#define I2S_BCLK 3
#define I2S_LRC 2
#define I2S_SD 9
#define GAIN_PIN 23

AudioGeneratorWAV *wav;
AudioFileSourceSD *file;
AudioOutputI2S *out;
bool isPlaying = false;
int activeLed = -1;
int planetsVisited = 0;
bool buttonStates[3] = {true, true, true}; // Track if buttons are active
bool isFirstRun = true;  // Track if this is the initial run after power on

void setup() {
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
  pinMode(button4Pin, INPUT_PULLUP);

  // Initialize SPI for SD card
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  Serial.println("SD card initialized successfully.");

  pinMode(GAIN_PIN, OUTPUT);
  digitalWrite(GAIN_PIN, HIGH);

  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  out->SetGain(1.0);

  wav = new AudioGeneratorWAV();

  // Play narratorstart.wav only if this is the first run
  if (isFirstRun) {
    playWavFile("/narratorstart.wav");
    while (isPlaying) stopPlayback();
    isFirstRun = false;  // Mark that the initial run has completed
  }
}

void playWavFile(const char* filename, int ledPin = -1) {
  if (isPlaying) return;

  file = new AudioFileSourceSD(filename);
  if (!file->isOpen()) {
    Serial.println("Error: Could not open WAV file!");
    return;
  }

  wav->begin(file, out);
  Serial.print("Playing: ");
  Serial.println(filename);
  if (ledPin != -1) {
    digitalWrite(ledPin, HIGH);
    activeLed = ledPin;
  }
  isPlaying = true;
}

void stopPlayback() {
  if (wav->isRunning()) {
    wav->loop();
  } else {
    wav->stop();
    file->close();
    Serial.println("Playback finished.");
    if (activeLed != -1) {
      digitalWrite(activeLed, LOW);
      activeLed = -1;
    }
    isPlaying = false;
  }
}

void resetGame() {
  planetsVisited = 0;
  buttonStates[0] = buttonStates[1] = buttonStates[2] = true;
  Serial.println("Game has been reset. Ready for a new adventure!");
}

void loop() {
  if (isPlaying) {
    stopPlayback();
    return;
  }

  if (planetsVisited == 0) {
    // Play narratorintro.wav at the start of a new run
    playWavFile("/narratorintro.wav");
    while (isPlaying) stopPlayback();
  }

  if (planetsVisited < 3) {
    if (buttonStates[0] && digitalRead(button1Pin) == LOW) {
      buttonStates[0] = false;
      playWavFile("/soundeffect.wav");
      delay(1000);

      playWavFile("/narratormercury.wav");
      while (isPlaying) stopPlayback();
      playWavFile("/novamercury.wav", ledone);
      while (isPlaying) stopPlayback();
      playWavFile("/zaramercury.wav", ledtwo);
      while (isPlaying) stopPlayback();
      playWavFile("/lunamercury.wav", ledthree);
      while (isPlaying) stopPlayback();
      playWavFile("/mercuryleaving.wav");

      planetsVisited++;
    }
    else if (buttonStates[1] && digitalRead(button2Pin) == LOW) {
      buttonStates[1] = false;
      playWavFile("/soundeffect.wav");
      delay(1000);

      playWavFile("/narratorvenus.wav");
      while (isPlaying) stopPlayback();
      playWavFile("/novavenus.wav", ledone);
      while (isPlaying) stopPlayback();
      playWavFile("/zaravenus.wav", ledtwo);
      while (isPlaying) stopPlayback();
      playWavFile("/lunavenus.wav", ledthree);
      while (isPlaying) stopPlayback();
      playWavFile("/venusleaving.wav");

      planetsVisited++;
    }
    else if (buttonStates[2] && digitalRead(button3Pin) == LOW) {
      buttonStates[2] = false;
      playWavFile("/soundeffect.wav");
      delay(1000);

      playWavFile("/narratormars.wav");
      while (isPlaying) stopPlayback();
      playWavFile("/novamars.wav", ledone);
      while (isPlaying) stopPlayback();
      playWavFile("/zaramars.wav", ledtwo);
      while (isPlaying) stopPlayback();
      playWavFile("/lunamars.wav", ledthree);
      while (isPlaying) stopPlayback();
      playWavFile("/marsleaving.wav");

      planetsVisited++;
    }
  }

  if (planetsVisited == 3) {
    playWavFile("/narratorvisitall.wav");
    while (isPlaying) stopPlayback();
    if (digitalRead(button1Pin) == LOW || digitalRead(button2Pin) == LOW || digitalRead(button3Pin) == LOW) {
      playWavFile("/soundeffect.wav");
      delay(1000);
      playWavFile("/narratorend.wav");
      while (isPlaying) stopPlayback();

      // Wait for any button press to restart the game
      while (true) {
        if (digitalRead(button1Pin) == LOW || digitalRead(button2Pin) == LOW || digitalRead(button3Pin) == LOW) {
          resetGame();
          break;
        }
        delay(100);
      }
    }
  }

  delay(100);
}
