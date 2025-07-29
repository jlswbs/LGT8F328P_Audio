// SID emulator - Pentatonic tone //

#include "SID.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

SID mySid;

  int randomNote = 1;
  int randomDuration = 2;
  int noteStep = 1;
  int notes[16];

void playSound(long frequency, long length) {

  mySid.setFrequency(0, frequency);
  mySid.setFrequency(1, frequency);
  mySid.setFrequency(2, frequency);
  
  mySid.noteOn(0);
  mySid.noteOn(1);
  mySid.noteOn(2);
  delay(50);
  mySid.noteOff(0);
  mySid.noteOff(1);
  mySid.noteOff(2);
  delay(length / 10);

}

void setup() {
  
  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  mySid.begin();

  mySid.setADSR(0, 30, 30, 200, 250); 
  mySid.setWaveForm(0, TRIANGLE);
  mySid.setADSR(1, 30, 30, 200, 250); 
  mySid.setWaveForm(1, TRIANGLE);
  mySid.setADSR(2, 30, 30, 200, 250); 
  mySid.setWaveForm(2, TRIANGLE);

  notes[1] = 131;
  notes[2] = 147;
  notes[3] = 165;
  notes[4] = 196;
  notes[5] = 220;
  notes[6] = 262;
  notes[7] = 294;
  notes[8] = 330;
  notes[9] = 392;
  notes[10] = 440;
  notes[11] = 523;
  notes[12] = 587;
  notes[13] = 659;
  notes[14] = 784;
  notes[15] = 880;

}

void loop() {
  
  noteStep = random(-3, 3);
  randomNote = randomNote + noteStep;
  if (randomNote < 1) {
    randomNote = random(1, 15);
  }
  else if (randomNote > 15) {
    randomNote = random(1, 15);
  }

  randomDuration = random(1, 8);
  int noteDuration = 3000 / randomDuration;
  
  playSound(notes[randomNote], noteDuration);

  int pauseBetweenNotes = noteDuration * 1.30;
  delay(10);

  LED_PORT ^= 1 << LED_BIT;

}