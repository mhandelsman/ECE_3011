void checkButtons() {
  if (!isPlaying) {
    if (digitalRead(button1Pin) == LOW) {
      playWavFile("/ledone.wav", ledone);
    } else if (digitalRead(button2Pin) == LOW) {
      playWavFile("/ledtwo.wav", ledtwo);
    } else if (digitalRead(button3Pin) == LOW) {
      playWavFile("/ledthree.wav", ledthree);
    }
  }
}
