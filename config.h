#ifndef CONFIG_H
#define CONFIG_H

#ifdef CONFIG_INCLUDE_MENU_CONFIGS
#include <termbox2.h>
static const uintattr_t menu_background = TB_BLACK;
static const uintattr_t menu_background_reversed = TB_WHITE;
static const uintattr_t menu_foreground = TB_WHITE;
static const uintattr_t menu_foreground_reversed = TB_BLACK;
static const float menu_split = 0.5f;
#endif

#ifdef CONFIG_INCLUDE_PLAYLISTS_PATH
static const char* playlists_path = "/path/to/your/playlists";
#endif

#ifdef CONFIG_INCLUDE_STROKES
#include <argument.h>
#include <stroke_functions.h>
struct Stroke {
  const char* string;
  int (*function) (struct Argument*);
  struct Argument argument;
};

static const char* strokes_allowed_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$%^&*()_+[]{}\\|;':\",./<>?";

static const struct Stroke stroke_bindings[] = {
  { "q", stroke_function_quit }
};

#endif

#endif
