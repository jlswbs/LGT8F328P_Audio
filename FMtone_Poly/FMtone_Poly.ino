// Polyphonic 2op FM synth //

#include "fmtone.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define BPM         120
    
// (op1) FB , Atk ,Decy, SL, Sus, Rel, TL, Mul, Ws ,(op2) Atk , Decy, SL, Sus, Rel, TL, Mul, Ws
static const uint8_t sample_tone[17] = {3, 15, 9, 4, 3, 0, 17, 3, 1, 15, 8, 5, 3, 8, 0, 5, 0};

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

  FMTONE.set_midistate(MIDI_POLY);
  FMTONE.midi_set_tone(sample_tone);

}

void loop() {

  LED_PORT ^= 1 << LED_BIT;
 
  FMTONE.midi_command(0x90, 0x90, 24 + rand()%32, 64 + rand()%48);
  FMTONE.midi_command(0x90, 0x90, 32 + rand()%48, 64 + rand()%48);
  FMTONE.midi_command(0x90, 0x90, 24 + rand()%48, 64 + rand()%48);
  FMTONE.midi_command(0x90, 0x90, 48 + rand()%48, 64 + rand()%48);
  FMTONE.midi_command(0x90, 0x90, 24 + rand()%64, 64 + rand()%48);

  int tempo = 60000 / BPM;
  delay(tempo / 4);
    
}