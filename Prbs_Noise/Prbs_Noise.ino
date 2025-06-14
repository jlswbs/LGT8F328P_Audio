// Noise base Pseudo-Random Bit Sequence generator //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 44100

class Synth {
public:

  unsigned long int reg = 0x55aa55aaL;
	unsigned long int calculate();

};

unsigned long int Synth::calculate() {

  unsigned long int newr;
  unsigned char lobit;
  unsigned char b31, b29, b25, b24;
  
  b31 = (reg & (1L << 31)) >> 31;
  b29 = (reg & (1L << 29)) >> 29;
  b25 = (reg & (1L << 25)) >> 25;
  b24 = (reg & (1L << 24)) >> 24;
  
  lobit = b31 ^ b29 ^ b25 ^ b24;
  newr = (reg << 1) | lobit;
  reg = newr;

  return reg;

}

Synth noise;

   
ISR(TIMER1_COMPA_vect) {

  DALR = noise.calculate();
  
  digitalWrite(LED_PIN, DALR & 1);
   
}

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

}

void loop() {
  
}