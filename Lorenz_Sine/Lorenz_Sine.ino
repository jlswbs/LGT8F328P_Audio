// Lorenz-Sine chaotic oscillator and trigger //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

  float s = 10.0f;
  float r = 28.0f;
  float b = 8.0f/3.0f;

  float x = 1.0f;
  float y = 0.0f;
  float z = 0.0f;

  float dt = 0.01f;

  long A = 0x7e66;
  long Y[3] = {0, 0x1209, 0};


void setup() {

  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);
  
}


void loop() {
  
  float nx = s * (y - x);
  float ny = (r * x) - y - (x * z);
  float nz = (x * y) - (b * z);

  x = x + nx * dt;
  y = y + ny * dt;
  z = z + nz * dt;

  Y[0] = ((A * Y[1]) >> 14) - Y[2];
  Y[2] = Y[1];
  Y[1] = Y[0];
  
  DALR = 128 + (Y[ 0 ] >> 8);

  int out = 2000 + (96.0f * x);

  if (out == 2000) LED_PORT ^= 1 << LED_BIT;
  
  delayMicroseconds(out);
  
}