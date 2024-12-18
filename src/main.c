#include <audio.h>
#include <change_song.h>
#include <config.h>
#include <display.h>
#include <init.h>
#include <free.h>
#include <event.h>
#include <main.h>

#include <orchestra/orchestra.h>
#include <tb_menu/tb_menu.h>

#include <stddef.h>
#include <stdlib.h>

const char** playlist_names = NULL;
unsigned int playlist_names_length = 0;

const char*** playlists = NULL;
unsigned int* playlists_lengths = NULL;

struct TbMenu playlist_menu;
struct TbMenu* playlists_menus = NULL;

struct TbMenuItem* playlist_menu_items = NULL;
struct TbMenuItem** playlists_menus_items = NULL;

unsigned int selected_change_song_function = 0;
unsigned int selected_menu = 0;
unsigned int selected_playlist = 1;
unsigned int* selected_songs = NULL;

bool running = true;

o_string strokes;

int main(void) {
  if(init() != EXIT_SUCCESS)
    return EXIT_FAILURE;

  tb_init();

  if(display_resize() != EXIT_SUCCESS)
    goto tb_shutdown;

  while(running) {
    if(!audio_is_playing())
      if(change_song() != EXIT_SUCCESS)
        goto tb_shutdown;

    if(display_draw() != EXIT_SUCCESS)
      goto tb_shutdown;

    struct tb_event event;
    tb_peek_event(&event, frame_rate);
    
    if(event_handle(&event) != EXIT_SUCCESS)
      goto tb_shutdown;
  }

tb_shutdown:
  tb_shutdown();
  free_all();
  return EXIT_SUCCESS;
}
