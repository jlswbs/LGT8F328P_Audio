// Knot geometric oscillator //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 44100

  float x = 0.0f;
  float y = 0.0f;
  float t = 0.0f;
  float a = 1.5f;
  float dt = 0.01f;
   
ISR(TIMER1_COMPA_vect) {

  DALR = 128 + (42 * x);

  float nx = x;
  float ny = y;
    
  x = (2.0f + sinf(ny * t)) * cosf(t);
  y = (2.0f + cosf(a * t)) * sinf(t);
    
  t = t + dt;

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