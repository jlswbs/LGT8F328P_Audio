// Lorenz 84 chaotic attractor as oscillator and chaotic trigger //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

  float a = 1.111f;
  float b = 1.479f;
  float f = 4.494f;
  float g = 0.44f;

  float x = 1.0f;
  float y = 1.0f;
  float z = 1.0f;

  float dt = 0.135f;


void setup() {

  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);
  
}


void loop() {
  
  float nx = x;
  float ny = y;
  float nz = z;
    
  x = nx + dt * (-a * nx - powf(ny, 2.0f) - powf(nz, 2.0f) + a * f);
  y = ny + dt * (-ny + nx * ny - b * nx * nz + g);
  z = nz + dt * (-nz + b * nx * ny + nx * nz);

  int out = 128 + (41 * x);

  if (out == 128) LED_PORT ^= 1 << LED_BIT;
  
  DALR = out;
  
}