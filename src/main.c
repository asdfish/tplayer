#include <display.h>
#include <init.h>
#include <free.h>
#include <main.h>

#include <orchestra.h>
#include <tb_menu.h>

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

unsigned int selected_menu = 0;
unsigned int selected_playlist = 0;

bool running = true;

o_string strokes;

int main(void) {
  if(init() != EXIT_SUCCESS)
    return EXIT_FAILURE;

  tb_init();

  if(display_resize() != EXIT_SUCCESS)
    goto tb_shutdown;

  while(running) {
    if(display_draw() != EXIT_SUCCESS)
      goto tb_shutdown;

    struct tb_event event;
    tb_poll_event(&event);
    
    if(event.ch == 'q')
      break;
  }

tb_shutdown:
  tb_shutdown();
  free_all();
  return EXIT_SUCCESS;
}
