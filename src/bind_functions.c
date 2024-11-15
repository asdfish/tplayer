#include <audio.h>
#include <config.h>
#include <change_song.h>
#include <display.h>
#include <bind_functions.h>
#include <macros.h>
#include <main.h>

static void get_selected_menu(struct TbMenu** menu, unsigned int* menu_length, unsigned int* selection);

static void get_selected_menu(struct TbMenu** menu, unsigned int* menu_length, unsigned int* selection) {
  if(selected_menu == 0) {
    if(menu != NULL)
      *menu = &playlist_menu;
    if(menu_length != NULL)
      *menu_length = playlist_names_length;
    if(selection != NULL)
      *selection = selected_playlist;
    return;
  }

  if(menu != NULL)
    *menu = playlists_menus + selected_playlist;
  if(menu_length != NULL)
    *menu_length = playlists_lengths[selected_playlist];
  if(selection != NULL)
    *selection = selected_songs[selected_playlist];
}

int bind_function_change_song(const struct Argument* argument) {
  return change_song();
}
int bind_function_change_song_function(const struct Argument* argument) {
  int next_function = argument->i;
  if(next_function < 0)
    next_function = 0;
  if((unsigned int) next_function > change_song_functions_length)
    next_function = change_song_functions_length;

  selected_change_song_function = next_function;
  redraw_status_bar = true;
  return EXIT_SUCCESS;
}
int bind_function_change_song_function_step(const struct Argument* argument) {
  if((int) selected_change_song_function + argument->i >= 0)
    selected_change_song_function += argument->i;
  if(selected_change_song_function > change_song_functions_length)
    selected_change_song_function = change_song_functions_length;

  redraw_status_bar = true;
  return EXIT_SUCCESS;
}

int bind_function_menu_move_cursor_bottom(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;
  unsigned int focused_menu_length = 0;
  get_selected_menu(&focused_menu, &focused_menu_length, NULL);

  focused_menu->cursor = focused_menu_length == 0 ? 0 : focused_menu_length - 1;
  redraw_menus = true;
  return EXIT_SUCCESS;
}
int bind_function_menu_move_cursor_to_selection(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;
  unsigned int selection = 0;
  get_selected_menu(&focused_menu, NULL, &selection);

  focused_menu->cursor = selection;
  redraw_menus = true;
  return EXIT_SUCCESS;
}
int bind_function_menu_move_cursor_top(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;
  unsigned int focused_menu_length = 0;
  get_selected_menu(&focused_menu, &focused_menu_length, NULL);

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

  get_selected_menu(&focused_menu, NULL, NULL);

  if(tb_menu_move_cursor(focused_menu, argument->i) != TBM_SUCCESS)
    return EXIT_FAILURE;

  redraw_menus = true;

  return EXIT_SUCCESS;
}
int bind_function_menu_select(const struct Argument* argument) {
  if(selected_menu == 0) {
    display_change_selected_playlist(playlist_menu.cursor);

    redraw_status_bar = true;
    return EXIT_SUCCESS;
  }

  redraw_status_bar = true;
  return change_specific_song(playlists_menus[selected_playlist].cursor);
}

int bind_function_quit(const struct Argument* argument) {
  running = false;
  return EXIT_SUCCESS;
}
