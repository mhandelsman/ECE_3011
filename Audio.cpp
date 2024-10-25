#include "audio.h"

// Constructor: Initializes the audio object
Audio::Audio() {
  wav = new AudioGeneratorWAV();
  out = new AudioOutputI2S();
  out->SetGain(1.0);  // Set max volume by default
  isPlaying = false;
}

// Method to play a WAV file
void Audio::playWavFile(const char* filename) {
  if (isPlaying) return;  // Don't start a new file if something is already playing

  file = new AudioFileSourceSD(filename);
  if (!file->isOpen()) {
    Serial.println("Error: Could not open WAV file!");
    return;
  }

  wav->begin(file, out);
  isPlaying = true;
  Serial.print("Playing: ");
  Serial.println(filename);
}

// Method to stop audio playback
void Audio::stopPlayback() {
  if (wav->isRunning()) {
    wav->loop();  // Continue looping the current file
  } else {
    wav->stop();
    file->close();
    Serial.println("Playback finished.");
    isPlaying = false;
  }
}

// Method to check if audio is playing
bool Audio::isAudioPlaying() {
  return isPlaying;
}
