// Karplus-Strong and LP filter - floating point //

#define SAMPLE_RATE 22050
#define SIZE        256
#define OFFSET      16
#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define BPM         120

  float out = 0.0f;
  float last = 0.0f;
  float curr = 0.0f;
  float tmp = 0.0f;
  float delaymem[SIZE];
  uint8_t locat = 0;
  uint8_t bound = SIZE;
  float lowpass = 0.0f;
 
  const float noise[128] = {
    0.646,0.833,0.398,0.750,0.835,0.322,0.552,0.979,0.549,0.330,0.619,0.361,0.757,0.414,0.492,0.695,
    0.973,0.328,0.838,0.739,0.954,0.032,0.357,0.663,0.282,0.230,0.711,0.625,0.591,0.660,0.048,0.349,
    0.162,0.794,0.311,0.529,0.166,0.602,0.263,0.654,0.689,0.748,0.451,0.084,0.229,0.913,0.152,0.826,
    0.538,0.996,0.078,0.443,0.107,0.962,0.005,0.775,0.817,0.869,0.084,0.400,0.260,0.800,0.431,0.911,
    0.182,0.264,0.146,0.136,0.869,0.580,0.550,0.145,0.853,0.622,0.351,0.513,0.402,0.076,0.240,0.123,
    0.184,0.240,0.417,0.050,0.903,0.945,0.491,0.489,0.338,0.900,0.369,0.111,0.780,0.390,0.242,0.404,
    0.096,0.132,0.942,0.956,0.575,0.060,0.235,0.353,0.821,0.015,0.043,0.169,0.649,0.732,0.648,0.451,
    0.547,0.296,0.745,0.189,0.687,0.184,0.368,0.626,0.780,0.081,0.929,0.776,0.487,0.436,0.447,0.306}; 

  float randomf(float minf, float maxf) {return minf + random(1UL << 31)*(maxf - minf) / (1UL << 31);}  


ISR(TIMER1_COMPA_vect) {
    
  delaymem[locat++] = out;
  if (locat >= bound) locat = 0;
  curr = delaymem[locat];
  tmp = 0.5f * (last + curr);
  last = curr;
  out = out - (lowpass * (out - tmp));
  
  DALR = 255 * out;

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
  
  for (int i = 0; i < SIZE; i++) delaymem[i] = noise[i>>1];
  
  bound = random(OFFSET, SIZE);
  lowpass = randomf(0.049f, 0.99f);

  int tempo = 60000 / BPM;
  delay(tempo / 2);

}