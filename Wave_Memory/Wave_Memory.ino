// Polyphonic Wavetable synth //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define BPM         120

#include "envtone.h"
  
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

  ENVTONE.set_midistate(MIDI_POLY);
  ENVTONE.midi_setwave(FM2OP2);
  ENVTONE.midi_set_envelope(12, 9, 25, 2, 8);

}

void loop() {

  LED_PORT ^= 1 << LED_BIT;
 
  ENVTONE.midi_command(0x90, 0x90, 24 + rand()%48, 96);
  ENVTONE.midi_command(0x90, 0x90, 32 + rand()%48, 96);
  ENVTONE.midi_command(0x90, 0x90, 48 + rand()%48, 96);
  ENVTONE.midi_command(0x90, 0x90, 24 + rand()%64, 96);
        
  int tempo = 60000 / BPM;
  delay(tempo / 4);
   
}