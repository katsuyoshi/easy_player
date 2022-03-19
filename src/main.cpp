#include <Arduino.h>
#if defined(ARDUINO_M5STACK_Core2)
  #include <M5Core2.h>
#elif defined( ARDUINO_M5Stack_Core_ESP32 )
  #include <M5Stack.h>
#endif
#include "freq.h"
#include "melody.h"

#define REST_NOTE   128

#define A_PIN   2   
#define B_PIN   26
#define C_PIN   5

int notes_size = sizeof(notes) / sizeof(notes[0]);
int notes_index = 0;
int volume = 0;
int note = REST_NOTE;
int inp_pins[] = {A_PIN, B_PIN, C_PIN};

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

struct io_state {
  unsigned ex_value:1;
  unsigned value:1;
  unsigned changed:1;
};

io_state inp_states[3];

void read_io(bool update = true) {
  if (update) {
    M5.update();
  }
  for (int i = 0; i < 3; i++) {
    inp_states[i].ex_value = inp_states[i].value;
    inp_states[i].value = digitalRead(inp_pins[i]);
    inp_states[i].changed = inp_states[i].value ^ inp_states[i].ex_value;
  }
}

void setup_io() {
  pinMode(A_PIN, INPUT_PULLUP);
  pinMode(B_PIN, INPUT_PULLUP);
  pinMode(C_PIN, INPUT_PULLUP);
  read_io();
  read_io(false);
}

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Speaker.setVolume(volume);
  setup_io();
  display();
}

bool io_raised(io_state state) {
  return state.changed && state.value;
}

void loop() {
  read_io();
  if(M5.BtnA.wasPressed() || io_raised(inp_states[0])) {
    note = notes[notes_index];
    display();
    if (note == REST_NOTE) {
      M5.Speaker.mute();
    } else {
      int freq = frequencies[note];
      M5.Speaker.tone(freq);
    }
    if (++notes_index >= notes_size) notes_index = 0;
  }
  if (M5.BtnB.wasPressed() || io_raised(inp_states[1])) {
    volume = (volume + 1) % 11;
    M5.Speaker.setVolume(volume);
    display();
  }
  if(M5.BtnC.wasPressed() || io_raised(inp_states[2])) {
    M5.Speaker.mute();
    note = REST_NOTE;
    display();
  }
}
