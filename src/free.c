#include <free.h>
#include <main.h>

#include <stdlib.h>

#include <tb_menu.h>

 void free_playlist_names(void) {
  free(playlist_names);
  playlist_names = NULL;
}
 void free_playlist_names_contents(void) {
  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    free((char*) playlist_names[i]);
    playlist_names[i] = NULL;
  }
}

 void free_playlists(void) {
  free(playlists);
  playlists = NULL;
}
 void free_playlists_contents(void) {
  free_playlists_contents_until(playlist_names_length);
}
 void free_playlists_contents_until(unsigned int until) {
  for(unsigned int i = 0; i < until; i ++) {
    for(unsigned int j = 0; j < playlists_lengths[i]; j ++) {
      free((char*) playlists[i][j]);
      playlists[i][j] = NULL;
    }
    free(playlists[i]);
    playlists[i] = NULL;
  }
}

 void free_playlists_lengths(void) {
  free(playlists_lengths);
  playlists_lengths = NULL;
}

 void free_playlist_menu(void) {
  tb_menu_uninit(&playlist_menu);
}
 void free_playlists_menus(void) {
  free(playlists_menus);
  playlists_menus = NULL;
}
 void free_playlists_menus_contents(void) {
  free_playlists_menus_contents_until(playlist_names_length);
}
 void free_playlists_menus_contents_until(unsigned int until) {
  for(unsigned int i = 0; i < until; i ++)
    tb_menu_uninit(&playlists_menus[i]);
}

 void free_playlist_menu_items(void) {
  free(playlist_menu_items);
  playlist_menu_items = NULL;
}
 void free_playlists_menus_items(void) {
  free(playlists_menus_items);
  playlists_menus_items = NULL;
}
 void free_playlists_menus_items_contents(void) {
  free_playlists_menus_items_contents_until(playlist_names_length);
}
 void free_playlists_menus_items_contents_until(unsigned int until) {
  for(unsigned int i = 0; i < until; i ++) {
    free(playlists_menus_items[i]);
    playlists_menus_items[i] = NULL;
  }
}

 void free_all(void) {
  free_all_playlists();
  free_all_menu_items();
  free_all_menus();
  free_all_strokes();
}
 void free_all_playlists(void) {
  free_playlists_contents();
  free_playlists();

  free_playlist_names_contents();
  free_playlist_names();

  free_playlists_lengths();
}
 void free_all_menus(void) {
  free_playlist_menu();
  free_playlists_menus_contents();
  free_playlists_menus();
}
 void free_all_menu_items(void) {
  free_playlists_menus_items_contents();
  free_playlists_menus_items();

  free_playlist_menu_items();
}
void free_all_strokes(void) {
  o_string_uninit(&strokes);
}
