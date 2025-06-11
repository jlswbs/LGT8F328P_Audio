// Sine oscillator //

#include "oscillator.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define BPM         120


void setup() {

  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  oscillator::initOSC();
  oscillator::setOSCFrequency(100);

}

void loop() {
    
  LED_PORT ^= 1 << LED_BIT;

  oscillator::setOSCFrequency(random(100, 1000));
   
  int tempo = 60000 / BPM;
  delay(tempo / 2);
  
}