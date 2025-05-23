// Math sine wave //

  float delta = -PI;

void setup() {

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);
  
}


void loop() { 

  DALR = 128 + (127.0f * sinf(delta));

  if (delta > PI) delta = -PI;

  delta = delta + 0.05f;
  
}