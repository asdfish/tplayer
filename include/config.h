#ifndef CONFIG_H
#define CONFIG_H

#include <bind_functions.h>
#include <change_song.h>
#include <status_info.h>
#include <structs.h>

#include <termbox2.h>

extern const uintattr_t menu_background;
extern const uintattr_t menu_background_reversed;
extern const uintattr_t menu_foreground;
extern const uintattr_t menu_foreground_reversed;
extern const uintattr_t menu_selected_foreground;
extern const uintattr_t menu_selected_foreground_reversed;
extern const float menu_split;

extern const char* playlists_path;

extern const char* strokes_allowed_chars;
extern const struct StrokeBinding stroke_bindings[];
extern const unsigned int stroke_bindings_length;

extern const struct KeyBinding key_bindings[];
extern const unsigned int key_bindings_length;

extern const unsigned int frame_rate;

extern const struct ChangeSongFunctionConfig change_song_functions[];
extern const unsigned int change_song_functions_length;

extern const StatusInfo status_info[];
extern const unsigned int status_info_length;

extern const unsigned int status_info_play_percentage_max;

#endif
