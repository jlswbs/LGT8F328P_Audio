// Drum samples + grain effect //

#include "drum_samples.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define SAMPLE_RATE 16000
#define BPM         120

  uint8_t out;
  int cnt, size, type;
  bool trig = false;

  const unsigned int d_size = 100;
  int d_time, i, j;
  bool rev = false;
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

  DALR = grain(out);

  if (type == 0) { out = pgm_read_byte_near(BD + cnt); size = BDLen; }
  if (type == 1) { out = pgm_read_byte_near(CH + cnt); size = CHLen; }
  if (type == 2) { out = pgm_read_byte_near(CL + cnt); size = CLLen; }
  if (type == 3) { out = pgm_read_byte_near(CR + cnt); size = CRLen; }
  if (type == 4) { out = pgm_read_byte_near(OH + cnt); size = OHLen; }
  if (type == 5) { out = pgm_read_byte_near(RD + cnt); size = RDLen; }
  if (type == 6) { out = pgm_read_byte_near(RS + cnt); size = RSLen; }
  if (type == 7) { out = pgm_read_byte_near(SN + cnt); size = SNLen; }

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
  type = rand() % 8;
  d_time = random(1, 100);
  rev = rand() % 2;
   
  int tempo = 60000 / BPM;
  delay(tempo / 3);
  
}