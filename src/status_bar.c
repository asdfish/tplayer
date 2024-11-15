#include <config.h>
#include <status_bar.h>
#include <termbox2.h>
#include <string.h>

void status_bar_draw(void) {
  int terminal_width = tb_width();
  if(terminal_width < 0)
    return;

  // clear
  for(unsigned int i = 0; i < (unsigned int) terminal_width; i ++)
    tb_set_cell(i, 0, ' ', 0, 0);

  unsigned int x = 0;
  for(unsigned int i = 0; i < status_info_length; i ++) {
    tb_printf(x, 0, 0, 0, "%s", status_info[i].contents);
    x += strlen(status_info[i].contents);
  }
}
int status_bar_update(void) {
  for(unsigned int i = 0; i < status_info_length; i ++)
    if(status_info[i].set_function(&status_info[i]) != EXIT_SUCCESS)
      return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
