#include <config.h>
#include <status_bar.h>
#include <termbox2.h>
#include <string.h>

static void status_bar_print(unsigned int* x, const char* text) {
  tb_print(*x, 0, status_info_foreground, status_info_background, text);
  *x += strlen(text);
}

void status_bar_draw(void) {
  int terminal_width = tb_width();
  if(terminal_width < 0)
    return;

  // clear
  for(unsigned int i = 0; i < (unsigned int) terminal_width; i ++)
    tb_set_cell(i, 0, ' ', 0, 0);

  unsigned int x = 0;
  status_bar_print(&x, status_info_separator);
  for(unsigned int i = 0; i < status_info_length; i ++) {
    status_bar_print(&x, status_info[i].contents);
    status_bar_print(&x, status_info_separator);
  }
}
int status_bar_update(void) {
  for(unsigned int i = 0; i < status_info_length; i ++)
    if(status_info[i].set_function(&status_info[i]) != EXIT_SUCCESS)
      return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
