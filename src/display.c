#include <config.h>
#include <display.h>
#include <main.h>

#include <termbox2.h>

void display_change_menu_selection(unsigned int* old_selection, unsigned int new_selection, struct TbMenuItem* affected_items, unsigned int affected_items_length) {
  affected_items[*old_selection].foreground = menu_foreground;
  affected_items[*old_selection].foreground_reversed = menu_foreground_reversed;

  affected_items[new_selection].foreground = menu_selected_foreground;
  affected_items[new_selection].foreground_reversed = menu_selected_foreground_reversed;

  *old_selection = new_selection;
}

void display_change_selected_playlist(unsigned int new_playlist) {
  if(selected_playlist == new_playlist)
    return;

  display_change_menu_selection(&selected_playlist, new_playlist, playlist_menu_items, playlist_names_length);
  redraw_menus = true;
}
int display_change_selected_song(unsigned int new_song) {
  if(new_song == selected_songs[selected_playlist])
    goto display_change_song;

  display_change_menu_selection(&selected_songs[selected_playlist], new_song, playlists_menus_items[selected_playlist], playlists_lengths[selected_playlist]);

  redraw_menus = true;
display_change_song:
  return EXIT_SUCCESS;
}

bool redraw_menus = true;
int display_draw(void) {
  bool redrawn = false;

  if(redraw_menus) {
    struct TbMenu* menus[2] = {
      &playlist_menu,
      playlists_menus + selected_playlist
    };

    for(unsigned int i = 0; i < 2; i ++) {
      if(i == selected_menu)
        menus[i]->enable_reversed_colors = true;
      else
        menus[i]->enable_reversed_colors = false;

      if(tb_menu_draw(menus[i]) != TBM_SUCCESS)
        return EXIT_FAILURE;
    }

    redraw_menus = false;
    redrawn = true;
  }

  if(redrawn)
    tb_present();
  return EXIT_SUCCESS;
}
int display_resize(void) {
  int terminal_width = tb_width();
  int terminal_height = tb_height();

  if(terminal_width < 0 || terminal_height < 0)
    return EXIT_FAILURE;

  unsigned int playlist_menu_width = terminal_width * menu_split;
  unsigned int playlists_menus_width = terminal_width - playlist_menu_width;

  playlist_menu.x = 0;
  playlist_menu.y = 0;
  playlist_menu.width = playlist_menu_width;
  playlist_menu.height = terminal_height;
  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    playlists_menus[i].x = playlist_menu_width;
    playlists_menus[i].y = 0;
    playlists_menus[i].width = playlists_menus_width;
    playlists_menus[i].height = terminal_height;
  }

  return EXIT_SUCCESS;
}
