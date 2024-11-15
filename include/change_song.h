#ifndef CHANGE_SONG_H
#define CHANGE_SONG_H

typedef unsigned int (*ChangeSongFunction) (void);

extern int change_song(void);
extern int change_specific_song(unsigned int song);

extern unsigned int change_song_loop(void);
extern unsigned int change_song_next(void);
extern unsigned int change_song_random(void);

#endif
