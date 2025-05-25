// Logistic Glitch base a chaotic logistic map and IIR filter oscillator //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 44100
#define BPM         120

  long A = 0x7e66;
  long y[3] = {0,0x1209,0};

  float r = 3.73f;
  float x = 0.1f;
  int chaos = 0;


ISR(TIMER1_COMPA_vect) {

  y[0] = (((A + chaos) * y[1]) >> 14) - y[2];
  y[2] = y[1];
  y[1] = y[0];

  int out = 128 + (y[ 0 ] >> 8);

  if (out <= 128) LED_PORT ^= 1 << LED_BIT;
  
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

  float nx = x;
  x = r * nx * (1.0f - nx);
  chaos = 500 * x;

  int tempo = 60000 / BPM;
  delay(tempo / 2);
  
}