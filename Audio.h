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
