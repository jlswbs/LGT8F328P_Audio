// SID emulator - Shepard tone //

#include "SID.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

SID mySid;

  int f1, f2, f3;

void setup() {
  
  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  mySid.begin();

  mySid.setFrequency(0, f1);
  mySid.setWaveForm(0, RECTANGLE);
  mySid.setAmplitude(0, 250);

  mySid.setFrequency(1, f2);
  mySid.setWaveForm(1, RECTANGLE);
  mySid.setAmplitude(1, 250);

  mySid.setFrequency(2, f3);
  mySid.setWaveForm(2, RECTANGLE);
  mySid.setAmplitude(2, 250);

}

void loop() {
  
  f1++;
  if(f1>2000) f1 = 220;
  f2 = f2 + 4;
  if(f2>2000) f2 = 220;
  f3 = f3 + 8;
  if(f3>2000) f3 = 220;
  
  mySid.setFrequency(0, f1);
  mySid.setFrequency(1, f2);
  mySid.setFrequency(2, f3);

  delay(10);

  LED_PORT ^= 1 << LED_BIT;

}