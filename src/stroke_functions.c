#include <display.h>
#include <stroke_functions.h>
#include <macros.h>
#include <main.h>

int stroke_function_menu_move_cursor_bottom(const struct Argument* argument) {
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
  GET_MENUS(menus)

  if(tb_menu_move_cursor(menus[selected_menu], argument->i) != TBM_SUCCESS)
    return EXIT_FAILURE;

  redraw_menus = true;

  return EXIT_SUCCESS;
}
int stroke_function_quit(const struct Argument* argument) {
  running = false;
  return EXIT_SUCCESS;
}
