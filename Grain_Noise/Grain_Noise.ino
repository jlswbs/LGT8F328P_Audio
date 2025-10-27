// Granular noise synth with delay //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 22050
#define BUFFER_SIZE 512
#define DELAY_BUFFER_SIZE 1024


uint8_t audioBuffer[BUFFER_SIZE];
uint8_t delayBuffer[DELAY_BUFFER_SIZE];
volatile uint16_t grainPosition = 0;
volatile uint16_t grainLength = 32 << 8;
volatile uint16_t grainSpeed = 1 << 8;
volatile uint8_t outputSample = 0;
volatile uint16_t delayIndex = 0;

volatile uint8_t feedbackGain = 50;

ISR(TIMER1_COMPA_vect) {

  grainPosition += grainSpeed;

  if (grainPosition >= grainLength) grainPosition = (random(0, BUFFER_SIZE - (grainLength >> 8))) << 8;

  uint8_t index = grainPosition >> 8;
  outputSample = audioBuffer[index];

  uint8_t delayedSample = delayBuffer[delayIndex];
  uint16_t mixedSample = (uint16_t)-outputSample * (100 - feedbackGain) + (uint16_t)delayedSample * feedbackGain;
  delayBuffer[delayIndex] = (uint8_t)(mixedSample / 100);
  delayIndex = (delayIndex + 1) % DELAY_BUFFER_SIZE;

  outputSample = (outputSample >> 1) + (delayedSample >> 1);

  DALR = outputSample;

}

void setup() {

  randomSeed(analogRead(A0));

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

  for (int i = 0; i < BUFFER_SIZE; i++) {

    float t = (float)i / BUFFER_SIZE * 2 * PI;
    audioBuffer[i] = (uint8_t)(128.0f + 127.0f * sinf(t));

  }

}

void loop() {

  LED_PORT ^= 1 << LED_BIT;

  grainSpeed = (random(64, 1024)) << 6;
  grainLength = (random(16, 1024)) << 8;
  feedbackGain = random(0, 99);

  delay(random(1, 100));
  
}