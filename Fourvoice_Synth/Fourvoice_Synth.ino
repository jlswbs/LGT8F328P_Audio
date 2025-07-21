// Four voice polyphony synth + LP filter //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 20000
#define BPM         120

#include "synth.h"

synth osc;

void setup() {

  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  cli();

  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  OCR1A = F_CPU / SAMPLE_RATE;
  TIMSK1 |= _BV(OCIE1A);

  sei();

  //setupVoice( voice[0-3] , waveform[SINE,TRIANGLE,SQUARE,SAW,RAMP,NOISE] , pitch[0-127], envelope[ENVELOPE0-ENVELOPE3], length[0-127], mod[0-127, 64=no mod])
  
  osc.setupVoice(0, SINE, 60, ENVELOPE0, 120, 32);
  osc.setupVoice(1, SAW, 60, ENVELOPE1, 60, 64);
  osc.setupVoice(2, NOISE, 60, ENVELOPE2, 80, 64);
  osc.setupVoice(3, TRIANGLE, 60, ENVELOPE3, 100, 60);

  osc.filter->setResonance(0);
  osc.filter->setCutoffFreq(255);
  
}

void loop(){
 
  LED_PORT ^= 1 << LED_BIT;
  
  osc.mTrigger(0, 32 + rand()%24);
  osc.mTrigger(1, 48 + rand()%36);
  osc.mTrigger(2, 36 + rand()%36);
  osc.mTrigger(3, 48 + rand()%24);

  osc.filter->setCutoffFreq(100 + rand()%140);

  int tempo = 60000 / BPM;
  delay(tempo / 3);
  
}