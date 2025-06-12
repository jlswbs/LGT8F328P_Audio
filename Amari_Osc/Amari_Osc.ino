// Amari neuron - chemical oscillation //

#include "oscillator.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

  float u = 0.0f;
  float s_u = 0.0f;
  float h_u = 0.0f;
  float c_uu = 1.5f;
  float d_u = 0.0f;
  float v = 0.0f;
  float s_v = 0.0f;
  float h_v = -1.0f;
  float c_vv = 0.0f;
  float d_v = 0.0f;
  float c_uv = -1.5f;
  float c_vu = 1.5f;
  float beta = 20.0f;
  float dt = 0.005f;
  float c_n = 0.1f;

float sigmoid(float x, float beta) { return 1.0f / (1.0f + expf(-x * beta)); }


void setup() {

  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  oscillator::initOSC();
  oscillator::setOSCFrequency(100);

}

void loop() {

  float noise = 1.0f / 1000.0f * random(1000) - 0.5f;

  d_u = dt * (-u + s_u + h_u + c_uu * sigmoid(u, beta) + c_uv * sigmoid(v, beta) + c_n * noise);
  u = u + d_u;
  
  d_v = dt * (-v + s_v + h_v + c_vv * sigmoid(v, beta) + c_vu * sigmoid(u, beta));
  v = v + d_v;
   
  oscillator::setOSCFrequency(1000 + 440.0f * u);

}