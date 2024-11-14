#ifndef MAIN_H
#define MAIN_H

extern const char** playlist_names;
extern unsigned int playlist_names_length;

extern const char*** playlists;
extern unsigned int* playlists_lengths;

extern struct TbMenu playlist_menu;
extern struct TbMenu* playlists_menus;

extern struct TbMenuItem* playlist_menu_items;
extern struct TbMenuItem** playlists_menus_items;

#endif