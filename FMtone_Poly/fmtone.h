#include <avr/io.h>

#define SAMPLE_RATE       44100
#define MAX_MIDITONE      5
#define WAVE_TBL_SIZE     64
#define TONE_DATLEN       17
#define MAX_DEFAULT_TONE  6

static const uint8_t default_tone[MAX_DEFAULT_TONE][17] = {
 { 0,15, 0, 4, 0, 0, 17, 1, 4, 12, 9, 6, 2, 4, 3, 1, 0}, //Syns Trumpet
 { 2,15, 9, 4, 3, 0,17, 3, 1, 15, 8, 5, 3, 8, 0, 1, 0,}, // piano
 { 2,12 , 8 , 4 , 0 , 7 , 11 , 2 , 2 , 11 , 5 , 1 , 0 , 10 , 3 , 1 , 2 ,},
 { 0,15 , 0 , 10 , 4 , 0 , 20 , 3 , 5 , 10 , 0 , 1 , 2 , 3 , 0 , 1 , 0 ,},
 { 0,15 , 7 , 5 , 3 , 4 , 0 , 1 , 5 , 15 , 2 , 0 , 2 , 6 , 3 , 1 , 0 ,},
 { 0,15 , 0 , 4 , 0 , 7 ,20 , 3 , 0 ,13 ,12 , 6 , 0 ,10 , 3 , 1 , 1 ,}
};

enum midistat {MIDI_POLY, MIDI_MONO};
enum wavetype {SIN, CLIP_SIN, TRI, SAW, RECT, ABS_SIN};

class FmTone
{

  protected:


  private:

    uint8_t queue_cnt = 0;
    uint8_t voice_queue[MAX_MIDITONE];
    uint8_t voice_queue_top = 0;
    uint8_t voice_queue_tail = 0;

    uint8_t ch_midino[MAX_MIDITONE];
    uint8_t active_voice_num = 0;
    uint8_t conv_count(uint8_t );

  public:

    FmTone();

    midi_command(uint8_t com, char dat1, char dat2, char dat3);
    midi_setwave(wavetype wave);

    set_midistate(midistat);
    midistat get_midistate();

    set_wave(uint8_t, wavetype );
    set_envelope(uint8_t ch, uint8_t atk, uint8_t decy, uint8_t sul, uint8_t sus, uint8_t rel, uint8_t tl, uint8_t mul);

    set_tone(uint8_t ch , uint8_t *);
    midi_set_tone(uint8_t *);
    midi_pg_chg(uint8_t);

    change_atk(uint8_t ch, uint8_t atk);

    change_decy(uint8_t ch, uint8_t decy);
    change_sul(uint8_t ch, uint8_t sul);

    change_sus(uint8_t ch, uint8_t sus);
    change_rel(uint8_t ch, uint8_t rel);
    change_tl(uint8_t ch, uint8_t tl);
    change_mul(uint8_t ch, uint8_t mul);
    change_fb(uint8_t ch, uint8_t fb);
    copy_tone();

  private:

    midi_note_off(uint8_t);
    midistat midi_state = MIDI_POLY;

  public:

};

extern FmTone FMTONE;