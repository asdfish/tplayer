#include <event.h>
#include <main.h>
#include <stdlib.h>

int event_handle(struct tb_event* event) {
  if(event == NULL)
    return EXIT_FAILURE;

  if(event->ch == 'q')
    running = false;

  return EXIT_SUCCESS;
}
