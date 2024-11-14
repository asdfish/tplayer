#include <display.h>
#include <stroke_functions.h>
#include <macros.h>
#include <main.h>
#include <utils.h>

int stroke_function_menu_move_cursor_bottom(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;
  unsigned int focused_menu_length = 0;
  get_selected_menu(&focused_menu, &focused_menu_length);

  focused_menu->cursor = focused_menu_length == 0 ? 0 : focused_menu_length - 1;
  redraw_menus = true;
  return EXIT_SUCCESS;
}
int stroke_function_menu_move_cursor_top(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;
  unsigned int focused_menu_length = 0;
  get_selected_menu(&focused_menu, &focused_menu_length);

  focused_menu->cursor = 0;
  redraw_menus = true;
  return EXIT_SUCCESS;
}
int stroke_function_menu_move_cursor_x(const struct Argument* argument) {
  if((int) selected_menu + argument->i >= 0)
    selected_menu += argument->i;
  if(selected_menu > 1)
    selected_menu = 1;
  redraw_menus = true;
  return EXIT_SUCCESS;
}
int stroke_function_menu_move_cursor_y(const struct Argument* argument) {
  struct TbMenu* focused_menu = NULL;

  get_selected_menu(&focused_menu, NULL);

  if(tb_menu_move_cursor(focused_menu, argument->i) != TBM_SUCCESS)
    return EXIT_FAILURE;

  redraw_menus = true;

  return EXIT_SUCCESS;
}
int stroke_function_quit(const struct Argument* argument) {
  running = false;
  return EXIT_SUCCESS;
}
