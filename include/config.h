#ifndef CONFIG_H
#define CONFIG_H

#include <bind_functions.h>
#include <structs.h>

#include <termbox2.h>

extern const uintattr_t menu_background;
extern const uintattr_t menu_background_reversed;
extern const uintattr_t menu_foreground;
extern const uintattr_t menu_foreground_reversed;
extern const float menu_split;

extern const char* playlists_path;

extern const char* strokes_allowed_chars;
extern const struct StrokeBinding stroke_bindings[];
extern const unsigned int stroke_bindings_length;

extern const struct KeyBinding key_bindings[];
extern const unsigned int key_bindings_length;

#endif
