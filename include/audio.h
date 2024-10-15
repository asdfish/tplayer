#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

extern int audio_init();
extern int audio_play(const char* path);
extern bool audio_is_playing();
extern void audio_uninit();

#endif
