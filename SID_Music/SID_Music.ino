// SID emulator - simple music //

#include "SID.h"
#include "pitches.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

SID mySid;

int underworld_melody[] = {//Underworld melody

  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, 
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, 
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4, 
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4,NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0

};

int underworld_tempo[] = {//Underwolrd tempo

  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18,18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3

};

void buzz(long frequency, long length) {

  mySid.setFrequency(0, frequency);
  mySid.setFrequency(1, frequency / 2);
  
  mySid.noteOn(0);
  mySid.noteOn(1);

  delay(30);

  mySid.noteOff(rand() % 2);

  delay(2*length-30);

}

void setup() {

  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  mySid.begin();

  mySid.setADSR(0, 10, 30, 250, 250); 
  mySid.setWaveForm(0, RECTANGLE);
  mySid.setADSR(1, 10, 30, 200, 100); 
  mySid.setWaveForm(1, SAWTOOTH);

}

void loop() {

  int size = sizeof(underworld_melody) / sizeof(int);

  for (int thisNote = 0; thisNote < size; thisNote++) {

    LED_PORT ^= 1 << LED_BIT;

    int noteDuration = 1000 / underworld_tempo[thisNote];
    buzz(underworld_melody[thisNote], noteDuration);

  }

}