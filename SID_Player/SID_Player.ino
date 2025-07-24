// SID emulator - simple dump player //

#include "SID.h"
#include "Alternative_Fuel.h"

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

SID mySid;

void setup() {

  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  mySid.begin();

}

void loop() {

  for(uint16_t sidPointer=0;sidPointer<=sidLength;sidPointer++){

    for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){

      mySid.set_register(sidRegister, (pgm_read_byte(&sidData[(sidPointer+sidRegister)])));

    }

    LED_PORT ^= 1 << LED_BIT;

    delay(19);

    sidPointer = sidPointer + 24;

  }

}