#include <main.h>
#include <utils.h>

void get_selected_menu(struct TbMenu** menu, unsigned int* menu_length) {
  if(selected_menu == 0) {
    if(menu != NULL)
      *menu = &playlist_menu;
    if(menu_length != NULL)
      *menu_length = playlist_names_length;
    return;
  }

  if(menu != NULL)
    *menu = playlists_menus + selected_playlist;
  if(menu_length != NULL)
    *menu_length = playlists_lengths[selected_playlist];
}
