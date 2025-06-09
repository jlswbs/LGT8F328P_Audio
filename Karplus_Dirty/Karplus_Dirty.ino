// Karplus-Strong filter and XOR drive shaper //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define SAMPLE_RATE 22050
#define SIZE        256
#define OFFSET      16
#define BPM         160

  float r = 3.75f;
  float x = 0.1f;

  uint8_t out;
  int last = 0;
  int curr = 0;
  uint8_t delaymem[SIZE];
  uint8_t locat = 0;
  uint8_t bound = SIZE;
  uint8_t xout;
  int accum = 0;
  int lowpass = 0;
  bool shaper = false;

   
ISR(TIMER1_COMPA_vect) {

  DALR = out;
  
  if(shaper) delaymem[locat++] = out^xout;
  else delaymem[locat++] = -out;
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
  
  shaper = rand()%2;
  lowpass = map(xout, 0, SIZE-1, 4, 0);

  for (int i = 0; i < SIZE; i++){
    
    float nx = x;
    x = r * nx * (1.0f - nx);
      
    xout = 255.0f * x;  
    delaymem[i] = xout / i;
    
  }

  bound = map(xout, 0, SIZE-1, OFFSET, SIZE-1);
   
  int tempo = 60000 / BPM;
  delay(tempo / 2);
  
}