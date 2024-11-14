#ifndef BIND_FUNCTIONS_H
#define BIND_FUNCTIONS_H

#include <structs.h>

extern int bind_function_menu_move_cursor_bottom(const struct Argument* argument);
extern int bind_function_menu_move_cursor_top(const struct Argument* argument);
extern int bind_function_menu_move_cursor_x(const struct Argument* argument);
extern int bind_function_menu_move_cursor_y(const struct Argument* argument);
extern int bind_function_menu_move_bottom(const struct Argument* argument);
extern int bind_function_menu_select(const struct Argument* argument);
extern int bind_function_quit(const struct Argument* argument);

#endif