#define CONFIG_INCLUDE_STROKES
#include <config.h>
#include <display.h>
#include <event.h>
#include <macros.h>
#include <main.h>
#include <stdlib.h>

static int event_handle_strokes(struct tb_event* event);

static int event_handle_strokes(struct tb_event* event) {
  char concat[2];
  concat[0] = event->ch;
  concat[1] = '\0';

  if(strstr(concat, strokes_allowed_chars) != NULL) {
    o_string_cat(&strokes, concat);

    unsigned int max_stroke_length = 0;
    for(unsigned int i = 0; i < ARRAY_LENGTH(stroke_bindings); i ++)
      max_stroke_length = MAX(max_stroke_length, strlen(stroke_bindings[i].string));
  }

  return EXIT_FAILURE;
}

int event_handle(struct tb_event* event) {
  if(event == NULL)
    return EXIT_FAILURE;

  if(event->type == TB_EVENT_RESIZE) {
    display_resize();
    goto exit;
  }

  event_handle_strokes(event);

exit:
  return EXIT_SUCCESS;
}
