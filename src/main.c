#include <init.h>
#include <free.h>
#include <main.h>

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

int main(void) {
  if(init() != EXIT_SUCCESS)
    return EXIT_FAILURE;

  free_all();
  return EXIT_SUCCESS;
}
