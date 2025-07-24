// SID emulator - random DTMF //

#include "SID.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

int DTMF[13][2] = {
  {941,1336}, // frequencies for touch tone 0
  {697,1209}, // frequencies for touch tone 1
  {697,1336}, // frequencies for touch tone 2
  {697,1477}, // frequencies for touch tone 3
  {770,1209}, // frequencies for touch tone 4
  {770,1336}, // frequencies for touch tone 5
  {770,1477}, // frequencies for touch tone 6
  {852,1209}, // frequencies for touch tone 7
  {852,1336}, // frequencies for touch tone 8
  {852,1477}, // frequencies for touch tone 9
  {941,1209}, // frequencies for touch tone *
  {941,1477}, // frequencies for touch tone #
};

SID mySid;

void setup() {
  
  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  mySid.begin();

  mySid.setADSR(0, 1, 1, 255, 1); 
  mySid.setWaveForm(0, TRIANGLE);

  mySid.setADSR(2, 1, 1, 255, 1); 
  mySid.setWaveForm(2, TRIANGLE);

}

void playDTMF(uint8_t digit, uint16_t duration_ms){

  mySid.setFrequency(0,DTMF[digit][0]);
  mySid.setFrequency(2,DTMF[digit][1]);
  mySid.noteOn(0);
  mySid.noteOn(2);
  delay(duration_ms);
  mySid.noteOff(0);
  mySid.noteOff(2);
  delay(50);

}

void loop() {

  LED_PORT ^= 1 << LED_BIT;

  uint8_t digit = rand() % 10;
  uint16_t duration_ms = 10 + rand() % 500;

  playDTMF(digit, duration_ms);

}