// Karplus-Strong filter with cosine saturator //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define SAMPLE_RATE 44100
#define SIZE        384
#define OFFSET      16
#define BPM         120

  int out = 0;
  int last = 0;
  int curr = 0;
  uint8_t delaymem[SIZE];
  uint8_t locat = 0;
  uint8_t bound = SIZE;
  int accum = 0;
  int lowpass = 0;

const unsigned char metal_noise[SIZE] = {

  32, 64, 156, 8, 64, 36, 0, 128, 1, 224, 25, 129, 200, 24, 97, 248, 
  100, 135, 8, 96, 103, 153, 158, 115, 152, 73, 242, 100, 39, 2, 96, 31, 
  135, 153, 242, 120, 127, 254, 79, 195, 224, 79, 147, 156, 243, 56, 67, 240, 
  228, 15, 0, 224, 4, 224, 73, 132, 200, 72, 100, 249, 36, 147, 9, 32, 
  115, 156, 159, 51, 200, 76, 243, 36, 51, 3, 32, 16, 225, 9, 144, 201, 
  8, 112, 252, 36, 195, 12, 32, 64, 228, 9, 192, 204, 8, 64, 60, 2, 
  64, 12, 0, 32, 0, 72, 1, 224, 16, 128, 102, 4, 224, 114, 4, 8, 
  24, 97, 38, 31, 132, 56, 100, 132, 200, 112, 132, 2, 24, 25, 33, 158, 
  100, 38, 103, 147, 231, 57, 3, 152, 73, 36, 159, 36, 50, 100, 36, 194, 
  112, 36, 0, 152, 7, 32, 126, 28, 33, 230, 115, 159, 39, 2, 120, 124, 
  159, 255, 15, 228, 243, 63, 15, 0, 248, 19, 33, 62, 76, 36, 231, 51, 
  207, 51, 3, 56, 67, 36, 63, 12, 48, 228, 12, 192, 240, 12, 0, 56, 
  1, 8, 4, 224, 64, 129, 38, 16, 225, 50, 16, 8, 72, 100, 38, 79, 
  144, 57, 36, 144, 201, 48, 144, 2, 72, 28, 33, 206, 112, 39, 39, 195, 
  243, 60, 3, 200, 76, 36, 207, 48, 51, 36, 48, 195, 48, 48, 0, 200, 
  4, 8, 16, 225, 0, 132, 38, 64, 228, 50, 64, 9, 8, 112, 39, 15, 
  192, 60, 36, 192, 204, 48, 192, 3, 8, 16, 8, 64, 228, 0, 144, 39, 
  0, 240, 51, 0, 12, 8, 64, 9, 0, 240, 0, 192, 36, 0, 48, 3, 
  0, 0, 0, 8, 0, 24, 0, 24, 1, 32, 0, 96, 30, 0, 72, 4, 
  32, 0, 96, 102, 1, 224, 78, 0, 72, 28, 128, 66, 4, 8, 0, 24, 
  97, 129, 152, 73, 128, 120, 126, 4, 225, 14, 0, 72, 100, 129, 200, 76, 
  128, 66, 28, 32, 64, 132, 2, 0, 6, 96, 97, 134, 72, 96, 102, 121, 
  132, 153, 9, 128, 121, 158, 31, 147, 224, 78, 115, 132, 57, 3, 128, 18, 
  97, 33, 146, 73, 32, 114, 124, 132, 201, 12, 128, 66, 100, 33, 194, 76

};

#define FIXED(x, n) ((uint16_t)((float)(x) * ((uint32_t)1 << (n)) + .5))

static uint16_t mul_fix_u16(uint16_t x, uint16_t y){

  uint16_t result;

    asm volatile(
	    "mul  %B1, %B2\n\t"
	    "movw %A0, r0\n\t"
	    "ldi  r19, 0x80\n\t"
	    "clr  r18\n\t"
	    "mul  %A1, %A2\n\t"
	    "add  r19, r1\n\t"
	    "adc  %A0, r18\n\t"
	    "adc  %B0, r18\n\t"
	    "mul  %B1, %A2\n\t"
	    "add  r19, r0\n\t"
	    "adc  %A0, r1\n\t"
	    "adc  %B0, r18\n\t"
	    "mul  %A1, %B2\n\t"
	    "add  r19, r0\n\t"
	    "adc  %A0, r1\n\t"
	    "adc  %B0, r18\n\t"
	    "clr  r1"
        : "=&r" (result)
        : "r" (x), "r" (y)
        : "r18", "r19"
    );

    return result;

}

static uint16_t mul_high_bytes(uint16_t x, uint16_t y){ return (uint8_t)(x >> 8) * (uint8_t)(y >> 8); }

int16_t cos_fix(uint16_t x){

    uint16_t y, s;
    uint8_t i = (x >> 8) & 0xc0;
    x = (x & 0x3fff) << 1;
    if (i & 0x40) x = FIXED(1, 15) - x;
    x = mul_fix_u16(x, x) << 1;
    y = FIXED(1, 15) - x;
    s = FIXED(0.23361, 16) - mul_high_bytes(FIXED(0.019531, 17), x);
    s = FIXED(1, 15) - mul_fix_u16(x, s);
    s = mul_fix_u16(y, s);
    return (i == 0x40 || i == 0x80) ? -s : s;

}
   
ISR(TIMER1_COMPA_vect) {

  DALR = cos_fix(4 * out);
  
  delaymem[locat++] = -out;
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
  
  for (int i = 0; i < SIZE; i++) delaymem[i] = metal_noise[i];
  
  bound = random(OFFSET, SIZE);
  lowpass = random(0, 5);
   
  int tempo = 60000 / BPM;
  delay(tempo / 3);
  
}