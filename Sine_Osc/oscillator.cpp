#include "arduino.h"
#include "oscillator.h"


void oscillator::processInterrupt();
void oscillator::initOSC();	
void oscillator::setOSCFrequency(float frequency);

bool oscillator::pendingRequest;
bool oscillator::interruptDisabled;
float oscillator::pendingFrequency;
float oscillator::normalisationFactor;
float oscillator::frequency;
uint8_t oscillator::indexWidth;
uint8_t oscillator::i;
uint16_t oscillator::timerICR;

	
void oscillator::initOSC(){
  
	pendingRequest = false;
	interruptDisabled = true;
	pendingFrequency = 0;
	normalisationFactor = 1.0;
	frequency = 0;
	timerICR = 0;
	indexWidth = 1;
	i=0;
	TCCR1A = (1<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<0)|(0<<0)|(1<<WGM11) |(0<<WGM10);
  TCCR1B = (0<<ICNC1) |(0<<ICES1) |(1<<WGM13) |(1<<WGM12) |(0<<CS12)    |(1<<CS11)  |(0<<CS10);
	TIMSK1=0x20;
	TCNT1 = 0;
	
}

void oscillator::applyOSCFrequency(){
  
  if(pendingFrequency>4000) indexWidth = 64;
  else if(pendingFrequency>2000) indexWidth = 32;
  else if(pendingFrequency>1000) indexWidth = 16;
  else if(pendingFrequency>500) indexWidth = 8;
  else if(pendingFrequency>250) indexWidth = 4;
  else if(pendingFrequency>125) indexWidth = 2;
  else indexWidth=1;
  timerICR = (int)(7700.0/(float)(pendingFrequency/(float)indexWidth));
  ICR1 = (int)timerICR;
  normalisationFactor = timerICR/255.0;

}

void oscillator::setOSCFrequency(float frequency){
  
  pendingRequest = true;
  pendingFrequency = frequency;
  if(interruptDisabled) applyOSCFrequency();
  
}

void oscillator::processInterrupt(){
  
  if(interruptDisabled) interruptDisabled = false;

		if(pendingRequest){
      pendingRequest = false;
      applyOSCFrequency();
    }
    DALR = sinewave_coeff[i]*normalisationFactor;
    i+=indexWidth;

}

ISR(TIMER1_CAPT_vect){
  
  oscillator::processInterrupt();
  
}