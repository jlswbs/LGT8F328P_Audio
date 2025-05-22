// 2x Karplus-Strong and Henon fractal //

#define SAMPLE_RATE 22050
#define BPM         120
#define SIZE        256
#define OFFSET      16
#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define GAIN        3

  int out1;
  int last1 = 0;
  int curr1 = 0;
  int16_t delaymem1[SIZE];
  uint8_t locat1 = 0;
  uint8_t bound1 = SIZE;
  int accum1 = 0;
  int lowpass1 = 2;

  int out2;
  int last2 = 0;
  int curr2 = 0;
  int16_t delaymem2[SIZE];
  uint8_t locat2 = 0;
  uint8_t bound2 = SIZE;
  int accum2 = 0;
  int lowpass2 = 2;

  float a = 1.4f;
  float b = 0.3f;
  float x = 1.0f;
  float y = 1.0f;

  int16_t clip = 0;


int16_t fold(int16_t x) {

    if(x > clip) { return fold((clip * 2) - x); }
    else if(x < -clip) { return fold((-clip * 2) - x); }
    else return x;

}
    

ISR(TIMER1_COMPA_vect) {
   
  DALR = 128 + (fold((out1 + out2) * GAIN) >> 8);

  delaymem1[locat1++] = -out1;
  if (locat1 >= bound1) locat1 = 0;
  curr1 = delaymem1[locat1];
  out1 = accum1 >> lowpass1;
  accum1 = accum1 - out1 + ((last1>>1) + (curr1>>1));
  last1 = curr1;
    
  delaymem2[locat2++] = -out2;
  if (locat2 >= bound2) locat2 = 0;
  curr2 = delaymem2[locat2];
  out2 = accum2 >> lowpass2;
  accum2 = accum2 - out2 + ((last2>>1) + (curr2>>1));
  last2 = curr2;
   
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
  float ny = y;
  
  x = 1.0f + ny - a * powf(nx, 2.0f);
  y = b * nx;

  int xout = 1000 * x;
  int yout = 1000 * y;

  clip = 100 * random(200, 240);
  
  for (int m = 0; m < SIZE; m++) delaymem1[m] = random(-32767, 32767);
  lowpass2 = random(1, 2);
  bound1 = map(xout, -1500, 1500, OFFSET, SIZE);

  int tempo = 60000 / BPM;
  delay(tempo / 4);
  
  for (int n = 0; n < SIZE; n++) delaymem2[n] = random(-32767, 32767);
  lowpass2 = random(1, 2);
  bound2 = map(yout, -400, 400, OFFSET, SIZE);
 
  delay(tempo / 4);

}