#include <config.h>
#include <macros.h>

const uintattr_t menu_background = TB_BLACK;
const uintattr_t menu_background_reversed = TB_WHITE;
const uintattr_t menu_foreground = TB_WHITE;
const uintattr_t menu_foreground_reversed = TB_BLACK;
const float menu_split = 0.5f;

const char* playlists_path = "/home/andre/files/music";

const char* strokes_allowed_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$%^&*()_+[]{}\\|;':\",./<>?";
const struct StrokeBinding stroke_bindings[] = {
  { "h", bind_function_menu_move_cursor_x, { .i = -1 } },
  { "j", bind_function_menu_move_cursor_y, { .i = 1 } },
  { "k", bind_function_menu_move_cursor_y, { .i = -1 } },
  { "l", bind_function_menu_move_cursor_x, { .i = 1 } },
  { "G", bind_function_menu_move_cursor_bottom },
  { "gg", bind_function_menu_move_cursor_top },
  { "q", bind_function_quit },
};
const unsigned int stroke_bindings_length = ARRAY_LENGTH(stroke_bindings);

const struct KeyBinding key_bindings[] = {
  { TB_KEY_ENTER, bind_function_menu_select },
};
const unsigned int key_bindings_length = ARRAY_LENGTH(key_bindings);
