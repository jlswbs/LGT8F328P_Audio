// Rainy noise generator //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define SAMPLE_RATE 44100

int16_t lp1, lp2, bp2, lowns, sah, rndm, lfrt;
   
ISR(TIMER1_COMPA_vect) {
  
  rndm = rand();
    
  if (lfrt-- == 0) {

    lfrt = (rndm & 511) + 700;
    sah = rndm;
    LED_PORT ^= 1 << LED_BIT;
    
  }

  bp2 = (sah/5 - bp2/16 - lp2) / 2 + bp2;
  lp2 = bp2/2 + lp2;                   
  lowns += (rndm - lowns) / 5  + (rndm / 40);
  lp1 += (rndm/4 - lp1) / 9;

  DALR = 128 + ((lp1/2 + bp2/3 + lowns/20) / 48);
   
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