#pragma GCC optimize("Ofast")
//~ #pragma GCC optimize("O3")
//~ #pragma GCC optimize("O2")
//~ #pragma GCC optimize("O1")
//~ #pragma GCC optimize("O0")

volatile const PROGMEM

#include "tune_free.h"
#include "mod_player.h"

#define MOD_DIRECT_PGM
#define AUDIO_SPS (F_CPU / 2560)
#define AUDIO_DT  (1000000L / AUDIO_SPS)

uint32_t t0 = 0;
uint32_t dt = 0;

mod_ctx ctx;

void setup() {

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	analogReference(DEFAULT);
	pinMode(DAC0, ANALOG);

	mod_init(&ctx, tune_mod, tune_mod_len, AUDIO_SPS);

	t0 = micros();

}

void loop() {

	if (ctx.err == 0) {

		int32_t sample = mod_render_sample(&ctx, dt);
		sample = sample / 256;

		DALR = (uint8_t)(128 + sample);

		digitalWrite(LED_BUILTIN, !(ctx.pattern_line % 4));
		
		while((micros()-t0) < AUDIO_DT);
		t0 = micros();

	}

}