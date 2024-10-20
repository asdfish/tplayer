#include <global.h>
#include <termbox2.h>

const char* menu_selection_prefix = "> ";

const unsigned int frame_rate = 1000 / 24;

unsigned int terminal_width, terminal_height;

unsigned int menu_selection_prefix_length;

// 0 random
// 1 order
unsigned int play_method = 0;

void global_change_play_method(void) {
  play_method ++;
  if(play_method > 1)
    play_method = 0;
}

void global_set_terminal_size(void) {
  int width_buffer = tb_width();
  int height_buffer = tb_height();
  terminal_width = (width_buffer > 0) ? (unsigned int) width_buffer : 0;
  terminal_height = (height_buffer > 0) ? (unsigned int) height_buffer : 0;
}

void global_set_menu_selection_prefix_length(void) {
  menu_selection_prefix_length = strlen(menu_selection_prefix);
}
