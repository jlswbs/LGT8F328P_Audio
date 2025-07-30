// SID emulator - random pitch from registers //

#include "SID.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define CONTROLREG  4
#define CHANNEL1    0
#define CHANNEL2    7
#define CHANNEL3    14
#define OFF         0
#define SETTRIANGLE_1	4, 0x11, 5, 0xBB, 6, 0xAA,
#define C4_1	1, 0x11, 0, 0x25,

SID mySid;

void setwaveform_triangle(uint8_t channel) {

  uint8_t dataset[]={SETTRIANGLE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     n+=2;
  }

}


void set_frequency(uint16_t pitch,uint8_t channel) {

    mySid.set_register(channel, pitch&0xFF);
    mySid.set_register(channel+1, pitch>>8);

}

void setup() {
  
  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  mySid.begin();

}
 
void loop() {

  setwaveform_triangle(CHANNEL1);
  setwaveform_triangle(CHANNEL2);

  set_frequency(400 + rand() % 4000, CHANNEL1);
  set_frequency(400 + rand() % 8000, CHANNEL2);

  LED_PORT ^= 1 << LED_BIT;

  delay(120);

}