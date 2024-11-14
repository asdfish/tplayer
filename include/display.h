#ifndef DISPLAY_H
#define DISPLAY_H

#include <tb_menu.h>
#include <stdbool.h>

extern bool redraw_menus;

extern void display_change_menu_selection(unsigned int* old_selection, unsigned int new_selection, struct TbMenuItem* affected_items, unsigned int affected_items_length);
extern void display_change_selected_playlist(unsigned int new_playlist);
extern int display_change_selected_song(unsigned int new_song);

extern int display_draw(void);
extern int display_resize(void);

#endif
