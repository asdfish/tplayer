#ifndef FREE_H
#define FREE_H

extern void free_playlist_names(void);
extern void free_playlist_names_contents(void);

extern void free_playlists(void);
extern void free_playlists_contents(void);
extern void free_playlists_contents_until(unsigned int until);

extern void free_playlists_lengths(void);

extern void free_playlist_menu(void);
extern void free_playlists_menus(void);
extern void free_playlists_menus_contents(void);
extern void free_playlists_menus_contents_until(unsigned int until);

extern void free_playlist_menu_items(void);
extern void free_playlists_menus_items(void);
extern void free_playlists_menus_items_contents(void);
extern void free_playlists_menus_items_contents_until(unsigned int until);

extern void free_status_info(void);
extern void free_status_info_until(unsigned int until);

extern void free_all(void);
extern void free_all_audio(void);
extern void free_all_playlists(void);
extern void free_all_menus(void);
extern void free_all_menu_items(void);
extern void free_all_strokes(void);
extern void free_all_selections(void);
extern void free_all_status_bars(void);

#endif
