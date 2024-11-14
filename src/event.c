#include <display.h>
#include <event.h>
#include <stroke.h>
#include <main.h>
#include <limits.h>
#include <stdlib.h>

int event_handle(struct tb_event* event) {
  if(event == NULL)
    return EXIT_FAILURE;

  if(event->type == TB_EVENT_RESIZE)
    if(display_resize() != EXIT_SUCCESS)
      return EXIT_FAILURE;

  if(event->ch < UCHAR_MAX)
    if(stroke_add_char((char) event->ch) != EXIT_SUCCESS)
      return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
