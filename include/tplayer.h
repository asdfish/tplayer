#ifndef TPLAYER_H
#define TPLAYER_H

extern const char** tplayer_playlist_names;
extern unsigned int tplayer_playlist_names_length;

extern const char*** tplayer_playlists;
extern unsigned int* tplayer_playlists_lengths;

extern struct TbMenu tplayer_playlist_menu;
extern struct TbMenu* tplayer_playlists_menus;

extern struct TbMenuItem* tplayer_playlist_menu_items;
extern struct TbMenuItem** tplayer_playlists_menus_items;


extern int tplayer(void);

#endif
