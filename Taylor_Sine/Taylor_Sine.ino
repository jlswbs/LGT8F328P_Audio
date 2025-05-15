// Taylor sine wave //

#define SAMPLE_RATE 44100

  float cnt = -PI;

float taylor(float angle) {

  float a2 = angle * angle;
  float x = angle;

  float rv = x;

  x = x * (1.0 / 6.0) * a2;
  rv -= x;

  x = x * (1.0 / 20.0) * a2;
  rv += x;

  x = x * (1.0 / 42.0) * a2;
  rv -= x;

  x = x * (1.0 / 72.0) * a2;
  rv += x;

  x = x * (1.0 / 110.0) * a2;
  rv -= x;

  return rv;

}

ISR(TIMER1_COMPA_vect) {

  DALR = 128 + (127.0f * taylor(cnt));

  if (cnt > PI) cnt = -PI;

  cnt = cnt + 0.1f;

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