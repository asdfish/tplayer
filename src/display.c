#define CONFIG_INCLUDE_MENU_CONFIGS
#include <config.h>
#include <display.h>
#include <main.h>

#include <termbox2.h>

int display_draw(void) {
  struct TbMenu* menus[2] = {
    &playlist_menu,
    playlists_menus + selected_menu
  };

  for(unsigned int i = 0; i < 2; i ++) {
    if(i == selected_menu)
      menus[i]->enable_reversed_colors = true;
    else
      menus[i]->enable_reversed_colors = false;

    if(tb_menu_draw(menus[i]) != TBM_SUCCESS)
      return EXIT_FAILURE;
  }

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

  playlist_menu.width = playlist_menu_width;
  playlist_menu.height = terminal_height;
  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    playlists_menus[i].x = playlist_menu_width;
    playlists_menus[i].width = playlists_menus_width;
    playlists_menus[i].height = terminal_height;
  }

  return EXIT_SUCCESS;
}
