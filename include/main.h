#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <status_info.h>
#include <orchestra/orchestra.h>
#include <tb_menu/tb_menu.h>

extern const char** playlist_names;
extern unsigned int playlist_names_length;

extern const char*** playlists;
extern unsigned int* playlists_lengths;

extern struct TbMenu playlist_menu;
extern struct TbMenu* playlists_menus;

extern struct TbMenuItem* playlist_menu_items;
extern struct TbMenuItem** playlists_menus_items;

extern unsigned int selected_change_song_function;
extern unsigned int selected_menu;
extern unsigned int selected_playlist;
extern unsigned int* selected_songs;

extern bool running;

extern o_string strokes;

#endif
