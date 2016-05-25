#include "pitches.h"
int sensorMin = 0;
int sensorMax = 10;
int touch_sensorMin = 70;
int touch_sensorMax = 500;
int tones_offset = 0;
long last_millis;
const int tones [] = {NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_B4,
                      NOTE_C5, NOTE_DS5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_B5,
                      NOTE_C6, NOTE_DS6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_B6};

const int measuresCount = 10;
const int tonesCount = sizeof(tones) / sizeof(tones[0]);

void setup() {
  Serial.begin(57600);
  pinMode(9, OUTPUT);
}

void loop() {
  int toneValue = 0;
  int ledValue = 0;
  int note_length = 0;
  int moisture_val = 0;
  int touch_val = analogRead(2);
  
  Serial.println(tones_offset);
  Serial.println(millis());  
  
  if(millis() - last_millis > 6000) {
    tones_offset++;
    tones_offset = tones_offset % 3;
    last_millis = millis();
    Serial.println(tones_offset);
  }
  
  for (int i = 0; i < measuresCount; i++) {
    moisture_val += analogRead(0);
  }
  moisture_val = moisture_val / measuresCount;

  if (moisture_val > sensorMax) {
    sensorMax = moisture_val;
  }
  
  if (touch_val > touch_sensorMax) {
    touch_sensorMax = touch_val;
  } else if (touch_val < touch_sensorMin) {
    touch_sensorMin = touch_val;
  }

  note_length = map(touch_val, touch_sensorMin, touch_sensorMax, 300, 50);

  if (moisture_val > 0) {

    toneValue = map(moisture_val, sensorMin, sensorMax, tones_offset * 6, (tones_offset * 6) + 6);
    ledValue = map(moisture_val, sensorMin, sensorMax, 0, 1023);

    tone(8, tones[toneValue], note_length);
    analogWrite(9, ledValue);

  } else {
    analogWrite(9, 0);
  }
  
  delay(note_length);

}
