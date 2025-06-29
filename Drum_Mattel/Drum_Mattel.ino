// Mattel drum samples //

#include "drum_samples.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define SAMPLE_RATE 16000
#define BPM         120

  uint8_t out;
  int cnt, size;
  bool trig = false;
  int type;

ISR(TIMER1_COMPA_vect) {

  DALR = out;

  if (type == 0) { out = pgm_read_byte_near(BD + cnt); size = BDLen; }
  if (type == 1) { out = pgm_read_byte_near(SD + cnt); size = SDLen; }
  if (type == 2) { out = pgm_read_byte_near(CY + cnt); size = CYLen; }
  if (type == 3) { out = pgm_read_byte_near(HT + cnt); size = HTLen; }
  if (type == 4) { out = pgm_read_byte_near(H3 + cnt); size = H3Len; }
  if (type == 5) { out = pgm_read_byte_near(LT + cnt); size = LTLen; }
  if (type == 6) { out = pgm_read_byte_near(CL + cnt); size = CLLen; }
  if (type == 7) { out = pgm_read_byte_near(CH + cnt); size = CHLen; }
  
  out >>= 1;
  out += 128;
  if (out > 255) out = 255;

  if (trig) { trig = false; cnt = 0; }
  if(cnt >= size) { trig = false; out = 0; }
  cnt++;

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

  trig = true;
  type = random(0, 8);
   
  int tempo = 60000 / BPM;
  delay(tempo / 3);
  
}