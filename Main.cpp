#include <Arduino.h>
#include "led.h"
#include "button.h"
#include "audio.h"
#include "sdcall.h"

// Pin definitions for LEDs
LED ledOne(10);
LED ledTwo(11);
LED ledThree(12);

// Pin definitions for Buttons
Button buttonOne(4);
Button buttonTwo(5);
Button buttonThree(15);
Button buttonFour(8);  // Button 4 defined but not used

// SD card and audio system
SDCall sd(18);  // CS pin for SD card
Audio audioSystem;

void setup() {
  Serial.begin(115200);

  // Initialize SD card
  if (!sd.initSD()) {
    Serial.println("Error initializing SD card!");
  }
}

void loop() {
  // If button 1 is pressed and no audio is playing, play ledone.wav
  if (buttonOne.isPressed() && !audioSystem.isAudioPlaying()) {
    ledOne.turnOn();
    audioSystem.playWavFile("/ledone.wav");
  }

  // If button 2 is pressed and no audio is playing, play ledtwo.wav
  else if (buttonTwo.isPressed() && !audioSystem.isAudioPlaying()) {
    ledTwo.turnOn();
    audioSystem.playWavFile("/ledtwo.wav");
  }

  // If button 3 is pressed and no audio is playing, play ledthree.wav
  else if (buttonThree.isPressed() && !audioSystem.isAudioPlaying()) {
    ledThree.turnOn();
    audioSystem.playWavFile("/ledthree.wav");
  }

  // Continue playing the file and stop when done
  if (audioSystem.isAudioPlaying()) {
    if (buttonOne.isPressed()) {
      audioSystem.stopPlayback();
      ledOne.turnOff();
    } else if (buttonTwo.isPressed()) {
      audioSystem.stopPlayback();
      ledTwo.turnOff();
    } else if (buttonThree.isPressed()) {
      audioSystem.stopPlayback();
      ledThree.turnOff();
    }
  }

  delay(100);  // Debounce delay
}
