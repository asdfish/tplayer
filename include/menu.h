#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

struct Menu {
  unsigned int x, y, width, height;

  unsigned int camera, cursor;

  bool enable_selections;
  unsigned int selection;

  const char** items;
  unsigned int* item_lengths;
  unsigned int items_length;
};

// free selections (if menu->enable_selections) and item_lengths before calling
extern int menu_change_items(struct Menu* menu, const char** items, unsigned int items_length);
extern void menu_draw(struct Menu* menu, bool draw_cursor);
// menu->items are not freed
extern void menu_free(struct Menu* menu);
extern void menu_move_cursor(struct Menu* menu, int step);
extern void menu_select(struct Menu* menu);

#endif
