// Karplus-Strong filtered distortion //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define SAMPLE_RATE 22050
#define BPM         120
#define SIZE        128
#define OFFSET      2

  int16_t out = 0;
  int16_t last = 0;
  int16_t curr = 0;
  int16_t lp = 0;
  int16_t bp = 0;
  int16_t delaymem[SIZE];
  uint8_t locat = 0;
  uint8_t bound = SIZE;

int16_t fuzz(int16_t x, uint8_t clip, uint8_t gain) {
  
  float y = x * gain;            
  if(y > clip) y = clip;
  else if(y < -clip) y = -clip;
  return y;

}
   
ISR(TIMER1_COMPA_vect) {

  DALR = 128 + fuzz(((lp + bp) >> 7), 64, 8);
  
  delaymem[locat++] = out;
  if (locat >= bound) locat = 0;
  curr = delaymem[locat];
  out = (last>>1) + (curr>>1);
  last = curr;

  bp += (out/4 - bp/16 - lp) / bound;
  lp += bp/4; 
   
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
  
  for (int i = 0; i < SIZE; i++) delaymem[i] = random(-32767, 32767);
  bound = random(OFFSET, SIZE);
   
  int tempo = 60000 / BPM;
  delay(tempo / 2);
  
}