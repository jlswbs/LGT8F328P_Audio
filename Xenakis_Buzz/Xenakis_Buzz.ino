/*

Based on stochastic synthesis methods (Xenakis 1971)
Programmed by Andrew R. Brown 2019

*/

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 44100

  unsigned long usec = micros();
  int state = 0;
  float dur = 2000.0f;
  float gain = 50.0f;
  float duty = 0.25f;

// volume gainVari (0-100, 500 max), 
// duration (pitch) Variation (0.0 - 10.0, 100 max), 
// duty cycle variation (0.0 - 1.0, larger vals possible).

void buzz (int gainVari, float durVari, float dutyVari) {

  if (state == 0) {

    gain += (random(gainVari * 2 + 1) - gainVari) / 4.0f;
    gain = fmin(120.0f, fmax(10.0f, gain));
    dur *= 1.0f + (random(durVari * 1000 + 1) - durVari * 500.0f) * 0.00001f;
    dur = fmin(20000.0f, fmax(400.0f, dur));
    duty += (random(2001) - 1000) * 0.00001f * dutyVari;
    duty = fmin(0.5f, fmax(0.01f, duty));
    DALR = gain;
    state = 1;
    usec += dur  * duty;

  } else {

    DALR = 0;
    state = 0;
    usec += dur * (1.0f - duty);

  }

}

ISR(TIMER1_COMPA_vect) {

  if (micros() > usec) buzz(2, 10.0f, 0.1f);

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