#include <audio.h>
#include <change_song.h>
#include <config.h>
#include <display.h>
#include <main.h>

int change_song(void) {
  unsigned int next_song = change_song_functions[selected_change_song_function]();
  return change_specific_song(next_song);
}
int change_specific_song(unsigned int song) {
  display_change_selected_song(song);
  redraw_menus = true;
  return audio_play(playlists[selected_playlist][song]);
}

unsigned int change_song_loop(void) {
  return selected_songs[selected_playlist];
}
unsigned int change_song_next(void) {
  unsigned int next_song = selected_songs[selected_playlist] + 1;
  return next_song >= playlists_lengths[selected_playlist] ? 0 : next_song;
}
unsigned int change_song_random(void) {
  return rand() % playlists_lengths[selected_playlist];
}
