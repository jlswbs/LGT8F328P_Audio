// IIR filter sine wave //

#define SAMPLE_RATE 44100

  long A=0x7e66;
  long y[3]={0,0x1209,0};

ISR(TIMER1_COMPA_vect) {

  y[ 0 ] = ((A * y[ 1 ]) >> 14 ) - y[ 2 ];
  y[ 2 ] = y[ 1 ];
  y[ 1 ] = y[ 0 ];
  
  DALR = 128 + (y[ 0 ] >> 8);

}

void setup() {

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