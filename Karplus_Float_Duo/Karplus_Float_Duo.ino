// Dual Karplus-Strong - floating point + LP filter //

#define SAMPLE_RATE 11025
#define BPM         120
#define SIZE        128
#define OFFSET      16
#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5


  float out1 = 0;
  float last1 = 0;
  float curr1 = 0;
  float tmp1 = 0;
  float delaymem1[SIZE];
  uint8_t locat1 = 0;
  uint8_t bound1 = SIZE;
  float LPF_Beta1 = 0.025f;

  float out2 = 0;
  float last2 = 0;
  float curr2 = 0;
  float tmp2 = 0;
  float delaymem2[SIZE];
  uint8_t locat2 = 0;
  uint8_t bound2 = SIZE;
  float LPF_Beta2 = 0.025f;
 
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

  DALR = 180.0f * (out1 + out2);
     
  delaymem1[locat1++] = out1;
  if (locat1 >= bound1) locat1 = 0;
  curr1 = delaymem1[locat1];
  tmp1 = 0.5f * (last1 + curr1);
  last1 = curr1;
  out1 = out1 - (LPF_Beta1 * (out1 - tmp1));
     
  delaymem2[locat2++] = out2;
  if (locat2 >= bound2) locat2 = 0;
  curr2 = delaymem2[locat2];
  tmp2 = 0.5f * (last2 + curr2);
  last2 = curr2;
  out2 = out2 - (LPF_Beta2 * (out2 - tmp2));
  
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
  
  for (int i = 0; i < SIZE; i++) delaymem1[i] = noise[i];
 
  bound1 = random(OFFSET, SIZE);

  LPF_Beta1 = randomf(0.049f, 0.499f);

  int tempo = 60000 / BPM;
  delay(tempo / 4);

  for (int i = 0; i < SIZE; i++) delaymem2[i] = noise[i];
  
  bound2 = random(OFFSET, SIZE);

  LPF_Beta2 = randomf(0.049f, 0.899f);

  delay(tempo / 3);

}