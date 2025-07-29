/*
 
 SID.cpp - Atmega8 MOS6581 SID Emulator
 
 Copyright (c) 2007 Christoph Haberer, christoph(at)roboterclub-freiburg.de
 Arduino Library Conversion by Mario Patino, cybernesto(at)gmail.com
 2015 Stereo capability added by Giovanni Giorgi, jj(at)gioorgi.com
 2016 direct access functions to underlying synthesizer by ch

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
/************************************************************************

	Atmega8 MOS6581 SID Emulator

	SID = Sound Interface Device

	This program tries to emulate the sound chip SID of the famous 
	historical C64 Commodore computer.
	The SID emulator includes all registers of the original SID, but 
	some functions may not be implemented yet.
	If you want	to program the SID registers to generate your own sound,
	please refer to the MOS6581 datasheet. The emulator tries to be as
	compatible as possible.

	In the main program there is an interrupt routine which sets the 
	ouptut values for the PWM-Output at 62.5kHz. Therefore the high 
	frequency noise	of the PWM should not be audible to normal people.
	The output is calculated with a 16kHz sample frequency to save 
	processing cycles.
	
	The envelope generators are updated every 1ms. 

	The amplitude value is output as an 8Bit PWM value. 
	The PWM-Output may be directly connected to an audio amplifier.

Giovanni Giorgi Stereo Add on

Simply put, the SID Voice2 is redirected to the PWM pin 10 (on ArduinoUno).
No additional load is expected/required
Is up to the client to know how to use this stereo capability.

	
************************************************************************

	Hardware
	
	processor:	ATMEGA
	clock:		16MHz crystal

	Arduino Uno PWM Audio Pins:
	
	Pin9, Pin10 // left, right audio output

***************************************************************************/

#include <avr/interrupt.h>
#include <Arduino.h>

#include "SID.h"

// attack, decay, release envelope timings
const static uint16_t AttackRate_ms[16]={2,4,16,24,38,58,68,80,100,250,500,800,1000,3000,5000,8000};
const static uint16_t DecayReleaseRate_ms[16]={6,24,48,72,114,168,204,240,300,750,1500,2400,3000,9000,15000,24000};
	
static uint8_t output;   // pin9
static uint8_t leftOutput;    // pin 10 GG Addon for StereoSID
static Sid_t Sid;
static Oscillator_t osc[OSCILLATORS];

/**
 * Initialize Arduino Register. Take control of Timer 1 and Timer 2.
 * Timer 1 will be used for PWM output (aka Analog-approximation)
 * Timer 2 will be used to compute output based on status of SID registers
 * 
 * OCR1A is an output compare register. It is constantly compared with a timer, which generates a PWM pulse.
 * The duty cycle of the PWM pulse is determined by the value of OCR1A. The PWM waveform is outputted to the OC1A pin.
 *
 * timer1A is pin 9   (11 on Mega)
 * timer1B is pin 10  (12 on Mega)	
 * Reference http://arduino.cc/en/Tutorial/SecretsOfArduinoPWM
 * for explanation of usage of _BV(COM1B1)
 * Additional information on 
 + http://www.avrfreaks.net/forum/tut-c-pwm-complete-idiots
 + http://sphinx.mythic-beasts.com/~markt/ATmega-timers.html
 + http://www.avrfreaks.net/forum/tut-c-newbies-guide-avr-pwm-incomplete?name=PNphpBB2&file=viewtopic&t=68302 
*/

void initialize(){

	// TIMER1 used to generate sound output
	// TIMER1: Fast PWM 8-bit
	TCCR1A = (1 << WGM10) | (1 << COM1A1) ; 
	// TIMER1: no prescaling
	TCCR1B = (1 << WGM12) | (1 << CS10);	
	// TIMER2 used to generate sample and ms interrupts
	// TIMER2: Normal Mode
	TCCR2A = 0 ;
	// TIMER2: clock/8 prescaling 
	TCCR2B = (1 << CS21);
	// TIMER2: set compare value to generate a 16kHz sample rate
	OCR2A = SAMPLERATECOUNT;
	// interrupt mask register: enable timer2 OCR2A interrupt
	TIMSK2 = (1 << OCIE2A);
	
	// interrupt mask register: enable timer1 overflow	
	TIMSK1 = (1 << TOIE1);
	
}

static int8_t wave(Voice_t *voice, uint16_t phase)
{
	int8_t out;
	uint8_t n = phase >> 8;
	uint8_t wavetype = voice->ControlReg;
	
	if(wavetype & SAWTOOTH)  
	{
		out = n - 128;
	}

	if(wavetype & TRIANGLE) 
	{
		if(n&0x80) 
			out = ((n^0xFF)<<1)-128;
		else 
			out = (n<<1)-128;
	}

	if(wavetype & RECTANGLE) 
	{
		if(n > (voice->PW >> 4)) // SID has 12Bit pwm, here we use only 8Bit
			out = 127;
		else 
			out = -127;
	}
		
	return out;
}

static uint8_t  remapDirectFilter(uint16_t temp, uint16_t temp1);

static void waveforms()
{
	static uint16_t phase[OSCILLATORS], sig[OSCILLATORS];
	static int16_t temp,temp1;
	static uint8_t i,j,k;
	static uint16_t noise = 0xACE1;
	static int8_t noise8;
	static uint16_t tempphase;

	// noise generator based on Galois LFSR
	noise = (noise >> 1) ^ (-(noise & 1) & 0xB400u);	
	noise8 = noise>>8;
	
	for(i = 0; i< OSCILLATORS; i++)
	{
		j = (i == 0 ? 2 : i - 1);
		tempphase=phase[i]+osc[i].freq_coefficient; //0.88us
		if(Sid.block.voice[i].ControlReg&NOISE)
		{				
			if((tempphase^phase[i])&0x4000) sig[i]=noise8*osc[i].envelope;			
		}
		else
		{
			if(Sid.block.voice[i].ControlReg&RINGMOD)
			{				
				if(phase[j]&0x8000) 
					sig[i]=osc[i].envelope*-wave(&Sid.block.voice[i],phase[i]);
				else 
					sig[i]=osc[i].envelope*wave(&Sid.block.voice[i],phase[i]);
			}
			else 
			{
				if(Sid.block.voice[i].ControlReg&SYNC)
				{
					if(tempphase < phase[j]) 
						phase[i] = 0;
				}
				else 
					sig[i]=osc[i].envelope*wave(&Sid.block.voice[i],phase[i]); //2.07us
			}
		}
		phase[i]=tempphase;
	}
	
	// voice filter selection
	temp=0; // direct output variable
	temp1=0; // filter output variable
	if(Sid.block.RES_Filt&FILT1) temp1+=sig[0];
	else temp+=sig[0];
	if(Sid.block.RES_Filt&FILT2) temp1+=sig[1];
	else temp+=sig[1];
	if(Sid.block.RES_Filt&FILT3) temp1+=sig[2];
	else if(!(Sid.block.Mode_Vol&VOICE3OFF))temp+=sig[2]; // voice 3 with special turn off bit

  k = remapDirectFilter(temp,temp1);
	output = k;

}
/** GG: This class merges direct and filter-ed output, anyway
 *      no real filter is implemented.
 */
static uint8_t  remapDirectFilter(uint16_t temp, uint16_t temp1){
  uint8_t k=(temp>>8)+128;
  k+=temp1>>10; // no real filter implemeted yet
  return k;
}

static void envelopes()
{
	uint8_t n;
	uint8_t controll_regadr[OSCILLATORS]={4,11,18};
	// if gate is ONE then the attack,decay,sustain cycle begins
	// if gate switches to zero the sound decays
	for(n=0;n<OSCILLATORS;n++)
	{
		if(Sid.sidregister[controll_regadr[n]]&GATE) // if gate set then attack,decay,sustain
		{
			if(osc[n].attackdecay_flag) 
			{	// if attack cycle
				osc[n].amp+=osc[n].m_attack;
				if(osc[n].amp>MAXLEVEL)
				{
					osc[n].amp=MAXLEVEL;
					osc[n].attackdecay_flag=false; // if level reached, then switch to decay
				}
			}
			else // decay cycle
			{
				if(osc[n].amp>osc[n].level_sustain)
				{
					osc[n].amp-=osc[n].m_decay;
					if(osc[n].amp<osc[n].level_sustain) osc[n].amp=osc[n].level_sustain;
				}

			} 
		}
		else // if gate flag is not set then release
		{
			osc[n].attackdecay_flag=true; // at next attack/decay cycle start wiht attack
			if(osc[n].amp>0)
			{
				osc[n].amp-=osc[n].m_release;
				if(osc[n].amp<0) osc[n].amp=0;
			}			
		}
		osc[n].envelope=osc[n].amp>>8;
	}
}

/************************************************************************

	interrupt routine timer 1 overflow
	- set PWM output

	Any calculation in this loop would mean to much processing load
	for the 16Mhz Atmega. Therefore a slower intterupt is needed
	
************************************************************************/
ISR(TIMER1_OVF_vect){

	DALR = output; // Output to PWM

}


/************************************************************************

	interrupt routine timer 2 
	
	slower interrupt routine with 16kHz to reduce processing load
	
	- calculate waverform phases
	- calculate waveforms
	- calculate attack decay release (1kHz)
	
************************************************************************/

ISR(TIMER2_COMPA_vect)
{
	static uint8_t mscounter = 0;
 	
	OCR2A += SAMPLERATECOUNT; // Output to PWM
	waveforms(); //~36us
	
	if(mscounter++ >= MSCOUNT)
	{
		envelopes(); //~16us
		mscounter = 0;
	}

}


// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

void SID::begin()
{
	
	initialize();
		
	//initialize SID-registers	
	Sid.sidregister[6]=0xF0;
	Sid.sidregister[13]=0xF0;
	Sid.sidregister[20]=0xF0;

	
	// set all amplitudes to zero
	for(int n=0;n<OSCILLATORS;n++) {
		osc[n].attackdecay_flag=true;
		setenvelope(&Sid.block.voice[n]);
		osc[n].amp=0;
	}
}


// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries
/************************************************************************
	
	uint8_t set_sidregister(uint8_t regnum, uint8_t value)

	The registers of the virtual SID are set by this routine.
	For some registers it is necessary to transform the SID-register
	values to some internal settings of the emulator. 
	To select this registers and to start the calculation, the switch/
	case statement is used.
	For instance: If setting the SID envelope register, new attack, decay
	sustain times are calculated.
	If an invalid register is requested the returned value will be 0.

	4.2007 ch

************************************************************************/
uint8_t SID::set_register(uint8_t regnum, uint8_t value)
{
	if(regnum>NUMREGISTERS-1) 
		return 0;
		
	Sid.sidregister[regnum]=value;

	switch(regnum)
	{
		//voice1
		case 1:
			osc[0].freq_coefficient=((uint16_t)Sid.sidregister[0]+((uint16_t)Sid.sidregister[1]<<8))>>2;
			break;
		case 5: setenvelope(&Sid.block.voice[0]);break;
		case 6: setenvelope(&Sid.block.voice[0]);break;
		
		//voice2
		case 8:
			osc[1].freq_coefficient=((uint16_t)Sid.sidregister[7]+((uint16_t)Sid.sidregister[8]<<8))>>2;
			break;
		case 12: setenvelope(&Sid.block.voice[1]);break;
		case 13: setenvelope(&Sid.block.voice[1]);break;		
		
		//voice3
		case 15:
			osc[2].freq_coefficient=((uint16_t)Sid.sidregister[14]+((uint16_t)Sid.sidregister[15]<<8))>>2;
			break;
		case 19: setenvelope(&Sid.block.voice[2]);break;
		case 20: setenvelope(&Sid.block.voice[2]);break;			
	}	
	return 1;
}

/************************************************************************
	
	uint8_t get_sidregister(uint8_t regnum)

	The registers of the virtual SID are read by this routine.
	If an invalid register is requested it returns zero.

************************************************************************/
uint8_t SID::get_register(uint8_t regnum)
{
	if(regnum>NUMREGISTERS-1)
		return 0;
    return Sid.sidregister[regnum];
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

uint8_t SID::get_wavenum(Voice_t *voice)
{
	uint8_t n;

	if(voice==&Sid.block.voice[0]) n=0;
	if(voice==&Sid.block.voice[1]) n=1;
	if(voice==&Sid.block.voice[2]) n=2;
	
	return n;
}

void SID::setfreq(Voice_t *voice,uint16_t freq)
{
	uint32_t templong;
	uint8_t n;

	n=get_wavenum(voice);
	
	templong=freq;
	osc[n].freq_coefficient=templong*4000/SAMPLEFREQ;
}

void SID::setenvelope(Voice_t *voice)
{
	uint8_t n;
	
	n=get_wavenum(voice);
	osc[n].attackdecay_flag=true;

	osc[n].level_sustain=(voice->SustainRelease>>4)*SUSTAINFACTOR;
	osc[n].m_attack=MAXLEVEL/AttackRate_ms[voice->AttackDecay>>4];
	osc[n].m_decay=(MAXLEVEL-osc[n].level_sustain*SUSTAINFACTOR)/DecayReleaseRate_ms[voice->AttackDecay&0x0F];
	osc[n].m_release=(osc[n].level_sustain)/DecayReleaseRate_ms[voice->SustainRelease&0x0F];
}


// Include a set of midi constant like
// const uint8_t A4=81; 440Hz
#include "midi_support.h"


uint16_t  SID::midi2Sid(uint8_t midin){
  return midin <=127 ? MIDI::midi2sidMapper[ midin ]:0;
}

void SID::play(uint8_t voice, uint16_t freq){
  byte low = freq & 0x00FF;
  byte high= freq >>8;
  set_register(voice+0,low);
  set_register(voice+1,high);
}

///////////////////////////////////////////////////////////////////////////////////
// direct access functions to underlying synthesizer //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void SID::setFrequency(uint8_t voiceNumber, uint16_t frequency_Hz)
{
	uint32_t fc=(uint32_t) frequency_Hz*0x10000/SAMPLEFREQ;

	cli();
	osc[voiceNumber].freq_coefficient=fc;
	sei();
}

/*
// waveform
#define TRIANGLE (1<<4)
#define SAWTOOTH (1<<5)
#define RECTANGLE (1<<6)
#define NOISE (1<<7)
*/
void SID::setWaveForm(uint8_t voiceNumber,uint8_t waveform)
{
	uint8_t temp;
	
	temp=Sid.block.voice[voiceNumber].ControlReg;
	temp&=0x0F; // clear upper bits
	temp|=waveform; // set waveform bits
	Sid.block.voice[voiceNumber].ControlReg=temp;
	if(waveform==RECTANGLE)
	{
		Sid.block.voice[voiceNumber].PW=512; // 50% pulse width
	}
}

// Before calling this function, call setWaveForm(uint8_t voiceNumber,RECTANGLE)
// pulseWidth=128 ==> 50% duty cycle
void SID::setPWM(uint8_t voiceNumber, uint8_t pulseWidth)
{
	Sid.block.voice[voiceNumber].PW=(uint16_t)pulseWidth<<4;
}

// https://en.wikipedia.org/wiki/Synthesizer#Attack_Decay_Sustain_Release_.28ADSR.29_envelope
void SID::setADSR(uint8_t voiceNumber,uint16_t attack_ms, uint16_t decay_ms, uint8_t sustain_level, uint16_t release_ms)
{
	uint8_t n=voiceNumber;

	Sid.block.voice[voiceNumber].ControlReg=Sid.block.voice[voiceNumber].ControlReg&=~GATE; // GATE OFF

	osc[n].level_sustain=(uint32_t)sustain_level*MAXLEVEL>>8; 
	osc[n].m_attack=MAXLEVEL/attack_ms;
	osc[n].m_decay=(MAXLEVEL-osc[n].level_sustain*SUSTAINFACTOR)/decay_ms;
	osc[n].m_release=(osc[n].level_sustain)/release_ms;

}

// start tone
void SID::noteOn(uint8_t voiceNumber)
{
	//osc[voiceNumber].attackdecay_flag=true;

	Sid.block.voice[voiceNumber].ControlReg=Sid.block.voice[voiceNumber].ControlReg|GATE; // GATE ON
}

void SID::noteOff(uint8_t voiceNumber)
{
	Sid.block.voice[voiceNumber].ControlReg=Sid.block.voice[voiceNumber].ControlReg&=~GATE; // GATE OFF
}

void SID::setAmplitude(uint8_t voiceNumber, uint16_t amplitude)
{
	// force envelope generator into release cycle
	Sid.block.voice[voiceNumber].ControlReg=Sid.block.voice[voiceNumber].ControlReg&=~GATE; // GATE OFF
	osc[voiceNumber].m_release=0; // prevent releasing in envelope loop
	osc[voiceNumber].amp=amplitude<<7;
}
/**
----------------
A Midnight Piano
----------------

A nice piano sound.  Works well if a second voice plays the same notes when detuned 10 to 20.

Waveform:      Pulse
Pulse Width:   2700
Attack:        2
Decay:         11
Sustain:       5
Release:       0
Release Point: 3



*/
void SID::loadPiano(uint8_t voice){
  configVoice(voice,RECTANGLE /*also pulse */,3700,0x2B,0x50,3);
  //configVoice(voice,RECTANGLE /*also pulse */,2700,0x2B,0x50,3);

}

/** Fast way of configuring a voice,  ADSR included. 
    ReleasePoint is unimplemented. 
    It seems how much 1/60 of seconds the note will last.
*/
    void SID::configVoice(uint8_t voice, uint8_t waveform, uint16_t pulsewidth /*12bit*/, uint8_t attackDecay, uint8_t sustainRelease, uint8_t releasePoint)
    {
      
      uint8_t pwlow = pulsewidth & 0x00FF;
      uint8_t pwhigh= pulsewidth >>8;
      set_register(voice+2 /*PW LOW*/,pwlow); set_register(voice+3,pwhigh);
      set_register(voice+5,attackDecay);
      set_register(voice+6,sustainRelease);
      // 33=%100001
      //          ^ Gate on (ADSR on)
      //     ^ Sawtooth Wave (Bit 5)
      set_register(voice+4, waveform | GATE);
     
      
    }
	
/* GG Follow complex functions */
void SID::playTestIntro() {
  loadPiano(VOICE1_Right);
  loadPiano(VOICE2_Left);
  loadPiano(VOICE3);
  set_register(24,15); // MAX VOLUME

  uint8_t i;
  for(i=MIDI::nC3; i<=MIDI::nC6; i+=2){
    
    play(VOICE1,midi2Sid(i));
    play(VOICE2,midi2Sid(MIDI::nA4));
    delay(90);
    play(VOICE2,midi2Sid(MIDI::nG4));
    play(VOICE1,midi2Sid(i+1));
    delay(90);  
  } 
  play(VOICE3,0); 
  play(VOICE2,0); 
  play(VOICE1,0); 
}
