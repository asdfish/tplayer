#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

extern int audio_init(void);
extern bool audio_is_playing(void);
extern int audio_play(const char* path);
extern void audio_uninit(void);

#endif
