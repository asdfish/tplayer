#include <free.h>
#include <tplayer.h>

#include <stdlib.h>

#include <tb_menu.h>

 void free_tplayer_playlist_names(void) {
  free(tplayer_playlist_names);
  tplayer_playlist_names = NULL;
}
 void free_tplayer_playlist_names_contents(void) {
  for(unsigned int i = 0; i < tplayer_playlist_names_length; i ++) {
    free((char*) tplayer_playlist_names[i]);
    tplayer_playlist_names[i] = NULL;
  }
}

 void free_tplayer_playlists(void) {
  free(tplayer_playlists);
  tplayer_playlists = NULL;
}
 void free_tplayer_playlists_contents(void) {
  free_tplayer_playlists_contents_until(tplayer_playlist_names_length);
}
 void free_tplayer_playlists_contents_until(unsigned int until) {
  for(unsigned int i = 0; i < until; i ++) {
    for(unsigned int j = 0; j < tplayer_playlists_lengths[i]; j ++) {
      free((char*) tplayer_playlists[i][j]);
      tplayer_playlists[i][j] = NULL;
    }
    free(tplayer_playlists[i]);
    tplayer_playlists[i] = NULL;
  }
}

 void free_tplayer_playlists_lengths(void) {
  free(tplayer_playlists_lengths);
  tplayer_playlists_lengths = NULL;
}

 void free_tplayer_playlist_menu(void) {
  tb_menu_uninit(&tplayer_playlist_menu);
}
 void free_tplayer_playlists_menus(void) {
  free(tplayer_playlists_menus);
  tplayer_playlists_menus = NULL;
}
 void free_tplayer_playlists_menus_contents(void) {
  free_tplayer_playlists_menus_contents_until(tplayer_playlist_names_length);
}
 void free_tplayer_playlists_menus_contents_until(unsigned int until) {
  for(unsigned int i = 0; i < until; i ++)
    tb_menu_uninit(&tplayer_playlists_menus[i]);
}

 void free_tplayer_playlist_menu_items(void) {
  free(tplayer_playlist_menu_items);
  tplayer_playlist_menu_items = NULL;
}
 void free_tplayer_playlists_menus_items(void) {
  free(tplayer_playlists_menus_items);
  tplayer_playlists_menus_items = NULL;
}
 void free_tplayer_playlists_menus_items_contents(void) {
  free_tplayer_playlists_menus_items_contents_until(tplayer_playlist_names_length);
}
 void free_tplayer_playlists_menus_items_contents_until(unsigned int until) {
  for(unsigned int i = 0; i < until; i ++) {
    free(tplayer_playlists_menus_items[i]);
    tplayer_playlists_menus_items[i] = NULL;
  }
}

 void free_all(void) {
  free_all_tplayer_playlists();
  free_all_menu_items();
  free_all_menus();
}
 void free_all_tplayer_playlists(void) {
  free_tplayer_playlists_contents();
  free_tplayer_playlists();

  free_tplayer_playlist_names_contents();
  free_tplayer_playlist_names();

  free_tplayer_playlists_lengths();
}
 void free_all_menus(void) {
  free_tplayer_playlist_menu();
  free_tplayer_playlists_menus_contents();
  free_tplayer_playlists_menus();
}
 void free_all_menu_items(void) {
  free_tplayer_playlists_menus_items_contents();
  free_tplayer_playlists_menus_items();

  free_tplayer_playlist_menu_items();
}
