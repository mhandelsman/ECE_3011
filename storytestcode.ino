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
bool introPlayed = false;  // Track if narratorintro.wav has been played in the current run
bool endPlayed = false; //Track if narratorvisitall.wav has been played in the current run

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

void flashAllLEDsDuringSoundEffect() {
  unsigned long startTime = millis();
  unsigned long duration = 3000;  // Set duration for LED flashing (e.g., 3 seconds)
  while (millis() - startTime < duration) {
    digitalWrite(ledone, HIGH);
    digitalWrite(ledtwo, HIGH);
    digitalWrite(ledthree, HIGH);
    delay(250);  // Flash interval
    digitalWrite(ledone, LOW);
    digitalWrite(ledtwo, LOW);
    digitalWrite(ledthree, LOW);
    delay(250);
  }
}

void resetGame() {
  planetsVisited = 0;
  introPlayed = false;  // Reset intro flag for a new game
  endPlayed = false;
  buttonStates[0] = buttonStates[1] = buttonStates[2] = true;
  Serial.println("Game has been reset. Ready for a new adventure!");
}

void loop() {
  if (isPlaying) {
    stopPlayback();
    return;
  }

  // Play narratorstart.wav only if this is the first run and wait for button press
  if (isFirstRun) {
    playWavFile("/narratorstart.wav");
    while (isPlaying) stopPlayback();
    
    // Wait for any button press to proceed
    while (digitalRead(button1Pin) == HIGH && digitalRead(button2Pin) == HIGH && digitalRead(button3Pin) == HIGH) {
      delay(100);  // Wait for button press
    }

    // Play the sound effect and flash LEDs
    playWavFile("/soundeffect.wav");
    flashAllLEDsDuringSoundEffect();
    while (isPlaying) stopPlayback();
    
    isFirstRun = false;  // Mark that the initial run has completed
  }

  // Play narratorintro.wav only once at the start of a new run
  if (planetsVisited == 0 && !introPlayed) {
    playWavFile("/narratorintro.wav");
    while (isPlaying) stopPlayback();
    introPlayed = true;  // Ensure intro doesn't replay within the same run
  }

  if (planetsVisited < 3) {
    if (buttonStates[0] && digitalRead(button1Pin) == LOW) {
      buttonStates[0] = false;
      playWavFile("/soundeffect.wav");
      flashAllLEDsDuringSoundEffect();
      delay(1000);

      playWavFile("/narratormercury.wav");
      while (isPlaying) stopPlayback();
      playWavFile("/novamercury.wav", ledone);
      while (isPlaying) stopPlayback();
      playWavFile("/zaramercury.wav", ledtwo);
      while (isPlaying) stopPlayback();
      playWavFile("/lunamercury.wav", ledthree);
      while (isPlaying) stopPlayback();
      planetsVisited++;
      if (planetsVisited != 3){
        playWavFile("/mercuryleaving.wav");
      } else {
        playWavFile("/nothing.wav");
      }
    }
    else if (buttonStates[1] && digitalRead(button2Pin) == LOW) {
      buttonStates[1] = false;
      playWavFile("/soundeffect.wav");
      flashAllLEDsDuringSoundEffect();
      delay(1000);

      playWavFile("/narratorvenus.wav");
      while (isPlaying) stopPlayback();
      playWavFile("/novavenus.wav", ledone);
      while (isPlaying) stopPlayback();
      playWavFile("/zaravenus.wav", ledtwo);
      while (isPlaying) stopPlayback();
      playWavFile("/lunavenus.wav", ledthree);
      while (isPlaying) stopPlayback();
      planetsVisited++;
      if (planetsVisited != 3){
        playWavFile("/venusleaving.wav");
      } else {
        playWavFile("/nothing.wav");
      }
    }
    else if (buttonStates[2] && digitalRead(button3Pin) == LOW) {
      buttonStates[2] = false;
      playWavFile("/soundeffect.wav");
      flashAllLEDsDuringSoundEffect();
      delay(1000);

      playWavFile("/narratormars.wav");
      while (isPlaying) stopPlayback();
      playWavFile("/novamars.wav", ledone);
      while (isPlaying) stopPlayback();
      playWavFile("/zaramars.wav", ledtwo);
      while (isPlaying) stopPlayback();
      playWavFile("/lunamars.wav", ledthree);
      while (isPlaying) stopPlayback();
      planetsVisited++;
      if (planetsVisited != 3){
        playWavFile("/marsleaving.wav");
      } else {
        playWavFile("/nothing.wav");
      }

    }
  }

  if (planetsVisited == 3 && !endPlayed) {
    playWavFile("/narratorvisitall.wav");
    while (isPlaying) stopPlayback();
    endPlayed = true;
    while (true){
      if (digitalRead(button1Pin) == LOW || digitalRead(button2Pin) == LOW || digitalRead(button3Pin) == LOW) {
        playWavFile("/soundeffect.wav");
        flashAllLEDsDuringSoundEffect();
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
      break;
    }
    delay(100);
    }
  }

  delay(100);
}
