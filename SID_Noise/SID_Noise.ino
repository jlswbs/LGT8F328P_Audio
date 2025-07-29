// SID emulator - Noise from registers //

#include "SID.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

#define CHANNEL1  0
#define CHANNEL2  7
#define CHANNEL3  14

#define C4_1	1, 0x11, 0, 0x25, // C4 tone pitch
#define OFF 0
#define SETNOISE_1 	4, 0x81, 5, 0xBB, 6, 0xAD, // SID register setup to create noise

SID mySid;

void setwaveform_noise(uint8_t channel){

  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0;  
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     n+=2; 
  }

}

void set_frequency(uint16_t pitch,uint8_t channel){

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
  
  setwaveform_noise(CHANNEL1);
  setwaveform_noise(CHANNEL2);
  setwaveform_noise(CHANNEL3);

  for(int n=100; n<1000; n+=100) {

    set_frequency(n*17, CHANNEL1);
    set_frequency(n*17, CHANNEL2);
    set_frequency(n*17, CHANNEL3);

    delay(1);

  }

  mySid.set_register(CONTROLREG + CHANNEL1, OFF);
  mySid.set_register(CONTROLREG + CHANNEL2, OFF);
  mySid.set_register(CONTROLREG + CHANNEL3, OFF);

  delay(100);

  LED_PORT ^= 1 << LED_BIT;

}