// Discrete time chaotic oscillator based Logistic equation //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 44100

  float a = 1.3f;
  float x = 0.15f;


ISR(TIMER1_COMPA_vect) {

  x = -a * (1.0f-x) * x / (fabs(x) + 0.01f);

  if (x > 2.65f) LED_PORT ^= 1 << LED_BIT;

  int out = 92 * x;

  DALR = out;

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