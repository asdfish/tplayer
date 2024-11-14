#define CONFIG_INCLUDE_MENU_CONFIGS
#include <config.h>
#include <display.h>
#include <main.h>

#include <termbox2.h>

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
    playlists_menus[i].width = playlists_menus_width;
    playlists_menus[i].height = terminal_height;
  }

  return EXIT_SUCCESS;
}
