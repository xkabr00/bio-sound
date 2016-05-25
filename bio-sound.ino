//Knihovna s definicemi nota - výška tónu
#include "pitches.h"

//Odpozorovaný rozsah vstupů ze senzoru vlhkosti
int sensorMin = 0;
int sensorMax = 10;

//Odpozorovaný rozsah vstupů z dotykového senzoru
int touch_sensorMin = 70;
int touch_sensorMax = 500;

const int scale_size = 6;
const int scale_change_interval = 3000;
int tones_offset = 0;
long last_millis;

//Předdefinované stupnice - blues hexatonic, ve třech oktávách 
const int tones [] = {NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_B4,
                      NOTE_C5, NOTE_DS5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_B5,
                      NOTE_C6, NOTE_DS6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_B6};

//Velikost zaokrouhlení pro vstup z vlhkostního senzoru
const int measuresCount = 10;

//const int tonesCount = sizeof(tones) / sizeof(tones[0]);

void setup() {
  Serial.begin(57600);

  //Natavení Pinu 9 - výstup pro LED
  pinMode(9, OUTPUT);
}

void loop() {
  int toneValue = 0;
  int ledValue = 0;
  int note_length = 0;
  int moisture_val = 0;
  
  //Přiřazení vstupu z dotykového senzoru
  int touch_val = analogRead(2);
  
 //Změna oktávy po daném časovém úseku
  if(millis() - last_millis > scale_change_interval) {
    tones_offset++;
    tones_offset = tones_offset % 3;
    last_millis = millis();
  }
 
//Zaokrouhlení vstupů
  for (int i = 0; i < measuresCount; i++) {
    moisture_val += analogRead(0);
  }
  moisture_val = moisture_val / measuresCount;

//Úprava rozsahu snímače vlhkosti
  if (moisture_val > sensorMax) {
    sensorMax = moisture_val;
  }

//Úprava rozsahu dotykového snímače
  if (touch_val > touch_sensorMax) {
    touch_sensorMax = touch_val;
  } else if (touch_val < touch_sensorMin) {
    touch_sensorMin = touch_val;
  }

//Mapování délky tónů na sílu stisku
  note_length = map(touch_val, touch_sensorMin, touch_sensorMax, 300, 50);

//moisture_val = 0 - žádný dotek
  if (moisture_val > 0) {
    
    //Mapování výšky tónů a intenzity LED na dotek
    toneValue = map(moisture_val, sensorMin, sensorMax, tones_offset * scale_size, (tones_offset * scale_size) + scale_size);
    ledValue = map(moisture_val, sensorMin, sensorMax, 0, 1023);

    //pin8 - piezo, pin9 - LED
    tone(8, tones[toneValue], note_length);
    analogWrite(9, ledValue);
  } else {
    //LED - zhasnutí
    analogWrite(9, 0);
  }
  
  delay(note_length);

}
