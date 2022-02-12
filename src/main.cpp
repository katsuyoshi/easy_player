#include <Arduino.h>
#include <M5Stack.h>
#include "freq.h"
#include "melody.h"

#define SILENT

#define EMPTY_NOTE   -1

int notes_size = sizeof(notes) / sizeof(notes[0]);
int notes_index = 0;
int volume = 1;
int note = EMPTY_NOTE;

void display() {
  uint32_t color = GREEN; //GREENYELLOW;
  // M5.Lcd.clear();

  // 背景表示
  M5.Lcd.fillRoundRect(0, 0, 320, 240, 4, color);

  if (0 <= note && note <= 127) {
    M5.Lcd.setCursor(120, 96);
    M5.Lcd.setTextSize(7);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.printf(note_names[note]);
  } else {
    M5.Lcd.fillRoundRect(110, 120, 100, 8, 2, BLACK);
    M5.Lcd.fillRect(130, 128, 60, 20,BLACK);
  }

  for (int i = 1; i <= 10; i++) {
    if (i <= volume) {
      M5.Lcd.drawRect(320 - 30, 240 - 30 - (i - 1) * 20, 20, 20, BLACK);
    } else {
      M5.Lcd.fillRect(320 - 30, 240 - 30 - (i - 1) * 20, 20, 20, BLACK);
    }
  }

}

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Speaker.setVolume(volume);
  display();
}

void loop() {
  M5.update();
  if(M5.BtnA.wasPressed()) {
    note = notes[notes_index];
    display();
    int freq = frequencies[note];
    if (++notes_index >= notes_size) notes_index = 0;
#ifndef SILENT
    M5.Speaker.tone(freq);
#endif
    M5.Speaker.setVolume(volume);
  }
  if (M5.BtnB.wasPressed()) {
    volume = (volume + 1) % 11;
    display();
  }
  if(M5.BtnC.wasPressed()) {
    M5.Speaker.mute();
    note = EMPTY_NOTE;
    display();
  }
}
