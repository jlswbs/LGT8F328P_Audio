// Lorenz chaotic attractor as oscillator and chaotic trigger //

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

  int out = 128 + (6.1f * x);

  if (out == 128) LED_PORT ^= 1 << LED_BIT;
  
  DALR = out;
  
}