// Fuzz-Strong filter and fuzz distortion //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define SAMPLE_RATE 22050
#define SIZE        256
#define OFFSET      16
#define BPM         120

  int out = 0;
  int last = 0;
  int curr = 0;
  int8_t delaymem[SIZE];
  uint8_t locat = 0;
  uint8_t bound = SIZE;
  int accum = 0;
  int lowpass = 0;

int8_t fuzz(int8_t x, uint8_t clip, uint8_t gain) {
  
  float y = x * gain;            
  if(y > clip) y = clip;
  else if(y < -clip) y = -clip;
  return y;

}
   
ISR(TIMER1_COMPA_vect) {

  DALR = 128 + out;
  
  delaymem[locat++] = fuzz(-out, 64, 8);
  if (locat >= bound) locat = 0;
  curr = delaymem[locat];
  out = accum >> lowpass;
  accum = accum - out + ((last>>1) + (curr>>1));
  last = curr;

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
  
  for (int i = 0; i < SIZE; i++) delaymem[i] = random(-127, 127);
  
  bound = random(OFFSET, SIZE);
  lowpass = random(0, 4);
   
  int tempo = 60000 / BPM;
  delay(tempo / 2);
  
}