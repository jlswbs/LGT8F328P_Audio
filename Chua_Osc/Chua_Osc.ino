// Chua chaotic oscillator //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 44100

  float x = 0.7f;
  float y = 0.0f;
  float z = 0.0f;
  float dt = 0.01f;
  float alpha = 15.6f * dt;
  float beta = -28.0f * dt; 
  float m0 = -1.143f;
  float m1 = -0.714f;
  float m2 = 0.5f * (m0 - m1);
  float m3 = 2.0f * m2;
  float h = 0.0f;


ISR(TIMER1_COMPA_vect) {

  float a = y - x;
  
  float xn = alpha * (a - h); 
  float yn = (z - a) * dt; 
  float zn = (beta) * y;

  if (x <= -1.0f) h = (m1 * x) - m3; 
  else if (x >= 1.0f) h = (m1 * x) + m3; 
  else h = (m1 + m3) * x;

  x = x + xn;  
  y = y + yn;
  z = z + zn;

  int out = 128 + (55 * x);

  if (out >= 128) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

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