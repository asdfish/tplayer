#include <config.h>
#include <macros.h>

const uintattr_t menu_background = TB_BLACK;
const uintattr_t menu_background_reversed = TB_WHITE;
const uintattr_t menu_foreground = TB_WHITE;
const uintattr_t menu_foreground_reversed = TB_BLACK;
const float menu_split = 0.5f;

const char* playlists_path = "/path/to/your/playlists";

const char* strokes_allowed_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$%^&*()_+[]{}\\|;':\",./<>?";

const struct Stroke stroke_bindings[] = {
  { "h", stroke_function_menu_move_cursor_x, { .i = -1 } },
  { "j", stroke_function_menu_move_cursor_y, { .i = 1 } },
  { "k", stroke_function_menu_move_cursor_y, { .i = -1 } },
  { "l", stroke_function_menu_move_cursor_x, { .i = 1 } },
  { "q", stroke_function_quit },
};
const unsigned int stroke_bindings_length = ARRAY_LENGTH(stroke_bindings);
