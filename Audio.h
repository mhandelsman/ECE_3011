#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>
#include "AudioOutputI2S.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"

class Audio {
  private:
    AudioGeneratorWAV *wav;
    AudioFileSourceSD *file;
    AudioOutputI2S *out;
    bool isPlaying;

  public:
    // Constructor: Initializes the Audio object
    Audio();

    // Method to play a WAV file
    void playWavFile(const char* filename);

    // Method to stop audio playback
    void stopPlayback();

    // Method to check if audio is playing
    bool isAudioPlaying();
};

#endif
