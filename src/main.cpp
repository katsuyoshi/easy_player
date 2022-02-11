#include <Arduino.h>
#include <M5Stack.h>
#include "freq.h"
#include "melody.h"

int notes_size = sizeof(notes) / sizeof(notes[0]);
int notes_index = 0;

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Speaker.setVolume(3);
}

void loop() {
  M5.update();
  if(M5.BtnA.wasPressed()) {
    int freq = frequencies[notes[notes_index]];
    if (++notes_index >= notes_size) notes_index = 0;
    M5.Speaker.tone(freq);
  }
  if(M5.BtnC.wasPressed()) {
    M5.Speaker.mute();
  }
}
