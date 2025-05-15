// Arduino polyphonic FM sound 
// * 31250 Hz sampling rate
// * 4-fold polyphony (4 different tones can play simulatenously)
// * FM-synthesis with time-varying modulation amplitude
// * ADSR envelopes
// * 12 preset instruments
// by Rolf Oldeman Feb 2019 
// Licence CC BY-NC-SA 2.5 https://creativecommons.org/licenses/by-nc-sa/2.5/

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5

//instrument definitions          
#define ninstr 12           //   piano xlphn guitar cmbll bell funky vibr metal violin bass trumpt harm
unsigned int ldness[ninstr]  = {   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,   32,   32}; // loudness   
unsigned int pitch0[ninstr]  = {   12,   12,   12,   12,   24,   24,    0,   12,   24,   12,   12,   24}; // pitch of key0         
unsigned int ADSR_a[ninstr]  = { 4096, 8192, 8192, 8192, 4096,  512,  512, 8192,  128,  128,  256,  256}; // attack parameter  
unsigned int ADSR_d[ninstr]  = {    8,   32,   16,   16,    8,   16,   16,    8,   16,   16,   64,   32}; // decay parameter   
unsigned int ADSR_s[ninstr]  = {    0,    0,    0,    0,    0,    0,    0,    0,  240,  240,  192,  192}; // sustain parameter 
unsigned int ADSR_r[ninstr]  = {   64,  128,   32,   32,   16,   32,   32,   32,   32,   32,   64,   64}; // release parameter 
unsigned int FM_inc[ninstr]  = {  256,  512,  768,  400,  200,   96,  528,  244,  256,  128,   64,  160}; // FM frequency wrt pitch
unsigned int FM_a1[ninstr]  =  {  128,  512,  512, 1024,  512,    0, 1024, 2048,  256,  256,  384,  256}; // FM amplitude start
unsigned int FM_a2[ninstr]  =  {   64,    0,  128,  128,  128,  512,  768,  512,  128,  128,  256,  128}; // FM amplitude end
unsigned int FM_dec[ninstr]  = {   64,  128,  128,  128,   32,  128,  128,  128,  128,  128,   64,   64}; // FM decay


//set up array with sine values in signed 8-bit numbers 
const float pi = 3.14159265;
char sine[256];

void setsine() {
  for (int i = 0; i < 256; ++i) {
    sine[i] = (sin(2 * 3.14159265 * (i + 0.5) / 256)) * 128;
  }
}

//setup frequencies/phase increments, starting at C3=0 to B6. (A4 is defined as 440Hz)
unsigned int tone_inc[48];

void settones() {
  for (byte i=0; i<48; i++){
    tone_inc[i]= 440.0 * pow(2.0, ( (i-21) / 12.0)) * 65536.0 / (32000000.0/512) + 0.5;
  }
}

void setup() {

  pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  noInterrupts();

  setsine();
  settones();

  TCCR1B = 0B00001001;

}


//initialize the main parameters of the pulse length setting
#define nch 4 //number of channels that can produce sound simultaneously
unsigned int phase[nch]  = {0,0,0,0};
int          inc[nch]    = {0,0,0,0};
byte         amp[nch]    = {0,0,0,0};
unsigned int FMphase[nch]= {0,0,0,0};
unsigned int FMinc[nch]  = {0,0,0,0};
unsigned int FMamp[nch]  = {0,0,0,0};

// main function (forced inline) to update the pulse length
inline void setPWM() __attribute__((always_inline));
inline void setPWM() {

  //wait for the timer to complete loop
  while ((TIFR1 & 0B00000001) == 0);

  //Clear(!) the overflow bit by writing a 1 to it
  TIFR1 |= 0B00000001;

  //increment the phases of the FM
  FMphase[0] += FMinc[0];
  FMphase[1] += FMinc[1];
  FMphase[2] += FMinc[2];
  FMphase[3] += FMinc[3];

  //increment the phases of the note
  phase[0] += inc[0];
  phase[1] += inc[1];
  phase[2] += inc[2];
  phase[3] += inc[3];

  //calculate the output value and set pulse width for timer2
  int val = sine[(phase[0]+sine[FMphase[0]>>8]*FMamp[0]) >> 8] * amp[0];
  val += sine[(phase[1]+sine[FMphase[1]>>8]*FMamp[1]) >> 8] * amp[1];
  val += sine[(phase[2]+sine[FMphase[2]>>8]*FMamp[2]) >> 8] * amp[2];
  val += sine[(phase[3]+sine[FMphase[3]>>8]*FMamp[3]) >> 8] * amp[3];

  DALR = 128 + (val >> 7);
  
}

//properties of each note played
byte         iADSR[nch]     = {0, 0, 0, 0}; 
unsigned int envADSR[nch]   = {0, 0, 0, 0}; 
unsigned int ADSRa[nch]     = {0, 0, 0, 0};
unsigned int ADSRd[nch]     = {0, 0, 0, 0};
unsigned int ADSRs[nch]     = {0, 0, 0, 0};
unsigned int ADSRr[nch]     = {0, 0, 0, 0};
byte         amp_base[nch]  = {0, 0, 0, 0};
unsigned int inc_base[nch]  = {0, 0, 0, 0};
unsigned int FMa0[nch]      = {0, 0, 0, 0};
int          FMda[nch]      = {0, 0, 0, 0};
unsigned int FMinc_base[nch]= {0, 0, 0, 0};
unsigned int FMdec[nch]     = {0, 0, 0, 0};
unsigned int FMexp[nch]     = {0, 0, 0, 0};
unsigned int FMval[nch]     = {0, 0, 0, 0};
byte         keych[nch]     = {0, 0, 0, 0}; 
unsigned int tch[nch]       = {0, 0, 0, 0}; 

// main loop. Duration of loop is determined by number of setPWM calls
// Each setPWMcall corresponds to 512 cylcles=32mus
// Tloop= 32mus * #setPWM. #setPWM=15 gives Tloop=0.48ms

int cnt;
byte instr = 0;
byte keypressed = 0;
byte keyreleased = 0;

void loop() {

  if (cnt == 1500) {
    
    keypressed = rand()%16;
    keyreleased = rand()%256;
    instr = rand()%12;
    cnt = 0;
    LED_PORT ^= 1 << LED_BIT;
    
  }
   
  cnt++; 

  setPWM(); //#1
  setPWM(); //#2

  //find the best channel to start a new note
  byte nextch = 255;
  //first check if the key is still being played
  if (iADSR[0] > 0 and keypressed == keych[0])nextch = 0;
  if (iADSR[1] > 0 and keypressed == keych[1])nextch = 1;
  if (iADSR[2] > 0 and keypressed == keych[2])nextch = 2;
  if (iADSR[3] > 0 and keypressed == keych[3])nextch = 3;
  //then check for an empty channel
  if (nextch == 255) {
    if (iADSR[0] == 0)nextch = 0;
    if (iADSR[1] == 0)nextch = 1;
    if (iADSR[2] == 0)nextch = 2;
    if (iADSR[3] == 0)nextch = 3;
  }
  //otherwise use the channel with the longest playing note
  if (nextch == 255) {
    nextch = 0;
    if (tch[0] > tch[nextch])nextch = 0;
    if (tch[1] > tch[nextch])nextch = 1;
    if (tch[2] > tch[nextch])nextch = 2;
    if (tch[3] > tch[nextch])nextch = 3;
  }

  setPWM(); //#3

  //initiate new note if needed
  if (keypressed != 255) {
    phase[nextch]=0;
    amp_base[nextch] = ldness[instr];
    inc_base[nextch] = tone_inc[pitch0[instr]+keypressed];
    ADSRa[nextch]=ADSR_a[instr];
    ADSRd[nextch]=ADSR_d[instr];
    ADSRs[nextch]=ADSR_s[instr]<<8;
    ADSRr[nextch]=ADSR_r[instr];
    iADSR[nextch] = 1;
    FMphase[nextch]=0;
    FMinc_base[nextch] = ((long)inc_base[nextch]*FM_inc[instr])/256;
    FMa0[nextch] = FM_a2[instr];
    FMda[nextch] = FM_a1[instr]-FM_a2[instr];
    FMexp[nextch]=0xFFFF;
    FMdec[nextch]=FM_dec[instr];
    keych[nextch] = keypressed;
    tch[nextch] = 0;
  }

  setPWM(); //#4

  //stop a note if the button is released
  if (keyreleased != 255) {
    if (keych[0] == keyreleased)iADSR[0] = 4;
    if (keych[1] == keyreleased)iADSR[1] = 4;
    if (keych[2] == keyreleased)iADSR[2] = 4;
    if (keych[3] == keyreleased)iADSR[3] = 4;
  }
  
  setPWM(); //#5

  //update FM decay exponential 
  FMexp[0]-=(long)FMexp[0]*FMdec[0]>>16;
  FMexp[1]-=(long)FMexp[1]*FMdec[1]>>16;
  FMexp[2]-=(long)FMexp[2]*FMdec[2]>>16;
  FMexp[3]-=(long)FMexp[3]*FMdec[3]>>16;
  
  setPWM(); //#6
  
  //adjust the ADSR envelopes
  for (byte ich = 0; ich < nch; ich++) {
    if (iADSR[ich] == 4) {
      if (envADSR[ich] <= ADSRr[ich]) {
        envADSR[ich] = 0;
        iADSR[ich] = 0;
      }
      else envADSR[ich] -= ADSRr[ich];
    }
    if (iADSR[ich] == 2) {
      if (envADSR[ich] <= (ADSRs[ich] + ADSRd[ich])) {
        envADSR[ich] = ADSRs[ich];
        iADSR[ich] = 3;
      }
      else envADSR[ich] -= ADSRd[ich];
    }
    if (iADSR[ich] == 1) {
      if ((0xFFFF - envADSR[ich]) <= ADSRa[ich]) {
        envADSR[ich] = 0xFFFF;
        iADSR[ich] = 2;
      }
      else envADSR[ich] += ADSRa[ich];
    }
    tch[ich]++;
    setPWM(); //#7-10
  }

  //update the tone for channel 0
  amp[0] = (amp_base[0] * (envADSR[0] >> 8)) >> 8;
  inc[0] = inc_base[0];
  FMamp[0] = FMa0[0] + ((long)FMda[0] * FMexp[0]>>16);
  FMinc[0] = FMinc_base[0];
  setPWM(); //#11

  //update the tone for channel 1
  amp[1] = (amp_base[1] * (envADSR[1] >> 8)) >> 8;
  inc[1] = inc_base[1];
  FMamp[1] = FMa0[1] + ((long)FMda[1] * FMexp[1]>>16);
  FMinc[1] = FMinc_base[1];
  setPWM(); //#12

  //update the tone for channel 2
  amp[2] = (amp_base[2] * (envADSR[2] >> 8)) >> 8;
  inc[2] = inc_base[2];
  FMamp[2] = FMa0[2] + ((long)FMda[2] * FMexp[2]>>16);
  FMinc[2] = FMinc_base[2];
  setPWM(); //#13

  //update the tone for channel 3
  amp[3] = (amp_base[3] * (envADSR[3] >> 8)) >> 8;
  inc[3] = inc_base[3];
  FMamp[3] = FMa0[3] + ((long)FMda[3] * FMexp[3]>>16);
  FMinc[3] = FMinc_base[3];
  setPWM(); //#14

  //update counters
  tch[0]++;
  tch[1]++;
  tch[2]++;
  tch[3]++;

  setPWM(); //#15

}