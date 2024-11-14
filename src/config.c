#include <config.h>
#include <macros.h>

const uintattr_t menu_background = TB_BLACK;
const uintattr_t menu_background_reversed = TB_WHITE;
const uintattr_t menu_foreground = TB_WHITE;
const uintattr_t menu_foreground_reversed = TB_BLACK;
const float menu_split = 0.5f;

const char* playlists_path = "/home/andre/files/music";

const char* strokes_allowed_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$%^&*()_+[]{}\\|;':\",./<>?";

const struct Stroke stroke_bindings[] = {
  { "q", stroke_function_quit }
};
const unsigned int stroke_bindings_length = ARRAY_LENGTH(stroke_bindings);
