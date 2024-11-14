#include <init.h>
#include <free.h>

#include <tb_menu.h>

const char** tplayer_playlist_names = NULL;
unsigned int tplayer_playlist_names_length = 0;

const char*** tplayer_playlists = NULL;
unsigned int* tplayer_playlists_lengths = NULL;

struct TbMenu tplayer_playlist_menu;
struct TbMenu* tplayer_playlists_menus = NULL;

struct TbMenuItem* tplayer_playlist_menu_items = NULL;
struct TbMenuItem** tplayer_playlists_menus_items = NULL;

int main(void) {
  if(init() != EXIT_SUCCESS)
    return EXIT_FAILURE;

  free_all();
  return EXIT_SUCCESS;
}
