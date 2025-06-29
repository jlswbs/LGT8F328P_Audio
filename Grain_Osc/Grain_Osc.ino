// Grain effect tuned IIR filter oscillator //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 44100
#define BPM         120
#define SIZE        100

  long A = 0x7e66;
  long y[3] = {0, 0x1209, 0};

  float r = 3.65f;
  float x = 0.1f;

  const unsigned int d_size = SIZE;
  int d_time, i, j;
  bool rev = true;
  char delay_1[d_size + 1] = { NULL };
  char delay_2[d_size + 1] = { NULL };

int grain(int d_in) {

  int d_out;
  i++; if (i  > d_time) i = 0;
  delay_1[i] = d_in;
  if (i == d_time) j = 0;
  delay_2[i]  = delay_1[i];
  j++; if (j > d_time) j = 0;
  if (!rev) d_out = delay_2[j];
  if (rev) d_out = delay_2[d_time - j];
  return d_out;

}

ISR(TIMER1_COMPA_vect) {

  y[0] = ((A * y[1]) >> 14) - y[2];
  y[2] = y[1];
  y[1] = y[0];

  DALR = grain(128 + (y[ 0 ] >> 8));

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

  LED_PORT ^= 1 << LED_BIT;

  float nx = x;
  x = r * nx * (1.0f - nx);
  d_time = SIZE * x;

  int tempo = 60000 / BPM;
  delay(tempo / 3);
  
}