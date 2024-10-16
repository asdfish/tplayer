#include <menu.h>
#include <global.h>

#include <termbox2.h>

#include <stdlib.h>

int menu_change_items(struct Menu* menu, const char** items, unsigned int items_length) {
  menu->items = items;
  menu->items_length = items_length;

  menu->item_lengths = (unsigned int*) malloc(items_length * sizeof(unsigned int));
  if(menu->item_lengths == NULL)
    return -1;
  for(unsigned int i = 0; i < items_length; i ++)
    menu->item_lengths[i] = strlen(items[i]);

  return 0;
}

void menu_draw(struct Menu* menu, bool draw_cursor) {
  if(menu->camera > menu->cursor)
    menu->camera = menu->cursor;
  else if(menu->camera + menu->height - 1 < menu->cursor)
    menu->camera = menu->cursor - menu->height + 1;

  for(unsigned int i = 0; i < menu->height; i ++) {
    unsigned int item = menu->camera + i;

    unsigned int max_x = (terminal_width < menu->x + menu->width) ? terminal_width - menu->x : menu->width;
    if(item >= menu->items_length) {
      for(unsigned int j = 0; j < max_x; j ++)
        tb_set_cell(menu->x + j, menu->y + i, ' ', MENU_NORMAL_FOREGROUND, MENU_NORMAL_BACKGROUND);
      continue;
    }

    uintattr_t foreground = (draw_cursor && menu->cursor == item) ? MENU_CURSOR_FOREGROUND : MENU_NORMAL_FOREGROUND;
    uintattr_t background = (draw_cursor && menu->cursor == item) ? MENU_CURSOR_BACKGROUND : MENU_NORMAL_BACKGROUND;

    const char* item_name = menu->items[item];
    unsigned int item_name_length = menu->item_lengths[item];

    bool selected = menu->enable_selections && item == menu->selection ? true : false;

    unsigned int x = menu->x;
    unsigned int y = menu->y + i;
    if(selected) {
      if(x + item_name_length + menu_selection_prefix_length < terminal_width) {
        tb_print(x, y, foreground, background, menu_selection_prefix);
        x += menu_selection_prefix_length;
        tb_print(x, y, foreground, background, item_name);
        x += item_name_length;
        goto draw_white_space;
      } else {
        for(unsigned int j = 0; j < menu_selection_prefix_length && x < terminal_width; j ++, x ++)
          tb_set_cell(x, y, menu_selection_prefix[j], foreground, background);
        for(unsigned int j = 0; j < item_name_length && x < terminal_width; j ++, x ++)
          tb_set_cell(x, y, item_name[j], foreground, background);
      }
    } else {
      if(x + item_name_length < terminal_width) {
        tb_print(x, y, foreground, background, item_name);
        x += item_name_length;
        goto draw_white_space;
      } else
        for(unsigned int j = 0; j < item_name_length && x < terminal_width; j ++, x ++)
          tb_set_cell(x, y, item_name[j], foreground, background);
    }

    if(x >= terminal_width)
      continue;

draw_white_space:
    for(; x < terminal_width; x ++)
      tb_set_cell(x, y, ' ', foreground, background);
  }
}

void menu_free(struct Menu* menu) {
  free(menu->item_lengths);
  menu->item_lengths = NULL;
}

void menu_move_cursor(struct Menu* menu, int step) {
  if((int) menu->cursor + step > 0)
    menu->cursor += step;
  else {
    menu->cursor = 0;
    return;
  }

  if(menu->cursor > menu->items_length - 1)
    menu->cursor = menu->items_length - 1;
}

void menu_select(struct Menu* menu) {
  if(menu->enable_selections)
    menu->selection = menu->cursor;
}
