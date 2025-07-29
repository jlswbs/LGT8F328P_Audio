// SID emulator - Amplitude modulation //

#include "SID.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

SID mySid;


void setup() {
  
  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  mySid.begin();

  mySid.setFrequency(0, 440);
  mySid.setWaveForm(0, TRIANGLE);
  mySid.setFrequency(1, 220);
  mySid.setWaveForm(1, TRIANGLE);

}

void loop() {
  
  int amplitude = 0;
  float loopTime = 1;
  float samplingTime = 0.01f;
  float phaseFactor = 4 * PI * samplingTime / loopTime;

  int counter = 0;
  
  while(1) {
    
    for (int n = 0; n < 255; n++) {
      amplitude = abs(250 * sinf(n * phaseFactor));
      mySid.setAmplitude(0, amplitude);
      mySid.setAmplitude(1, amplitude);
      delayMicroseconds(160);
    }

    LED_PORT ^= 1 << LED_BIT;
    
    counter++;

    if(counter>10) {
      mySid.setWaveForm(0, RECTANGLE);
      mySid.setWaveForm(1, RECTANGLE);
    }
    if(counter>20) { 
      mySid.setFrequency(0, 880);
      mySid.setFrequency(1, 440);
    }
    if(counter>30) {
      mySid.setFrequency(0, counter * 10);
      mySid.setFrequency(1, counter * 10);
    }
    if(counter>40) {
      mySid.setWaveForm(0, SAWTOOTH);
      mySid.setWaveForm(1, SAWTOOTH);
    }
    if(counter>50) counter=0;
    if(counter==0) {

      loopTime /= 2;
      phaseFactor = 4 * PI * samplingTime / loopTime;
      mySid.setWaveForm(0, TRIANGLE);
      mySid.setWaveForm(1, TRIANGLE);

    }
    
  }

}