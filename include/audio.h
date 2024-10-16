#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

extern int audio_init(void);
extern bool audio_is_playing(void);
extern int audio_play(const char* path);
extern float audio_play_percentage(void);
extern void audio_uninit(void);

#endif
