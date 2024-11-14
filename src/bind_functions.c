#include <display.h>
#include <bind_functions.h>
#include <macros.h>
#include <main.h>

static void change_selected_playlist(unsigned int new_playlist) {
  if(selected_playlist == new_playlist)
    return;

  change_menu_selection(&selected_playlist, new_playlist, playlist_menu_items, playlist_names_length);
  redraw_menus = true;
}

static void get_selected_menu(struct TbMenu** menu, unsigned int* menu_length) {
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

int bind_function_menu_move_cursor_bottom(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;
  unsigned int focused_menu_length = 0;
  get_selected_menu(&focused_menu, &focused_menu_length);

  focused_menu->cursor = focused_menu_length == 0 ? 0 : focused_menu_length - 1;
  redraw_menus = true;
  return EXIT_SUCCESS;
}
int bind_function_menu_move_cursor_top(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;
  unsigned int focused_menu_length = 0;
  get_selected_menu(&focused_menu, &focused_menu_length);

  focused_menu->cursor = 0;
  redraw_menus = true;
  return EXIT_SUCCESS;
}
int bind_function_menu_move_cursor_x(const struct Argument* argument) {
  if((int) selected_menu + argument->i >= 0)
    selected_menu += argument->i;
  if(selected_menu > 1)
    selected_menu = 1;
  redraw_menus = true;
  return EXIT_SUCCESS;
}
int bind_function_menu_move_cursor_y(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;

  get_selected_menu(&focused_menu, NULL);

  if(tb_menu_move_cursor(focused_menu, argument->i) != TBM_SUCCESS)
    return EXIT_FAILURE;

  redraw_menus = true;

  return EXIT_SUCCESS;
}
int bind_function_menu_select(const struct Argument* argument) {
  if(selected_menu == 0) {
    change_selected_playlist(playlist_menu.cursor);

    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
int bind_function_quit(const struct Argument* argument) {
  running = false;
  return EXIT_SUCCESS;
}
