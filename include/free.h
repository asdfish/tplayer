#ifndef FREE_H
#define FREE_H

extern void free_tplayer_playlist_names(void);
extern void free_tplayer_playlist_names_contents(void);

extern void free_tplayer_playlists(void);
extern void free_tplayer_playlists_contents(void);
extern void free_tplayer_playlists_contents_until(unsigned int until);

extern void free_tplayer_playlists_lengths(void);

extern void free_tplayer_playlist_menu(void);
extern void free_tplayer_playlists_menus(void);
extern void free_tplayer_playlists_menus_contents(void);
extern void free_tplayer_playlists_menus_contents_until(unsigned int until);

extern void free_tplayer_playlist_menu_items(void);
extern void free_tplayer_playlists_menus_items(void);
extern void free_tplayer_playlists_menus_items_contents(void);
extern void free_tplayer_playlists_menus_items_contents_until(unsigned int until);

extern void free_all(void);
extern void free_all_tplayer_playlists(void);
extern void free_all_menus(void);
extern void free_all_menu_items(void);

#endif