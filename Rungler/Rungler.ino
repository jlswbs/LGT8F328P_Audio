// Rungler - Rob Hordijk pattern generator //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 8000
#define BPM         120

  uint8_t tune;
  uint16_t feedback;
  uint16_t data;
  uint16_t shiftReg;
   
ISR(TIMER1_COMPA_vect) {

  DALR = shiftReg & 0xFF;

  feedback = shiftReg >> tune;
  feedback ^= data > 8000;
  shiftReg = shiftReg << 1;
  shiftReg |= feedback & 1;
  
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

  data += 32768;  // 1-65535 different pattern
  tune = 10;      // 1-15 value like bit reduction tune

  int tempo = 60000 / BPM;
  delay(tempo / 64);
  
}