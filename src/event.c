#include <config.h>
#include <display.h>
#include <event.h>
#include <macros.h>
#include <main.h>
#include <utils.h>
#include <limits.h>
#include <stdlib.h>

static bool stroke_valid_append(char* concat);
static int stroke_add_char(char new_char);
static int stroke_process(void);

static bool stroke_valid_append(char* concat) {
  return strstr(strokes_allowed_chars, concat) != NULL;
}
static int stroke_add_char(char new_char) {
  CHAR_TO_STRING(new_char, string)

  if(stroke_valid_append(string)) {
    if(o_string_cat(&strokes, string) != EXIT_SUCCESS)
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
static int stroke_process(void) {
  unsigned int max_length = stroke_bindings_max_length();
  unsigned int strokes_length = strlen(strokes.contents);

  bool clear_stroke = false;
  bool valid_stroke = false;

  if(strokes_length > max_length) {
    clear_stroke = true;
    goto clear_stroke;
  }

  for(unsigned int i = 0; i < stroke_bindings_length; i ++) {
    unsigned int stroke_binding_length = strlen(stroke_bindings[i].string);

    if(stroke_binding_length < strokes_length)
      continue;

    if(strcmp(strokes.contents, stroke_bindings[i].string) == 0) {
      if(stroke_bindings[i].function(&stroke_bindings[i].argument) != EXIT_SUCCESS)
        return EXIT_FAILURE;
      clear_stroke = true;
      goto clear_stroke;
    }

    if(strncmp(strokes.contents, stroke_bindings[i].string, MIN(strokes_length, stroke_binding_length)) == 0)
      valid_stroke = true;
  }

  if(!valid_stroke)
    clear_stroke = true;

clear_stroke:
  if(clear_stroke) {
    if(o_string_clear(&strokes) != EXIT_SUCCESS)
      return EXIT_FAILURE;
    return EXIT_SUCCESS;
  }
  return EXIT_SUCCESS;
}

int event_handle(struct tb_event* event) {
  if(event == NULL)
    return EXIT_FAILURE;

  if(event->type == TB_EVENT_RESIZE)
    if(display_resize() != EXIT_SUCCESS)
      return EXIT_FAILURE;

  if(event->ch < CHAR_MAX) {
    if(stroke_add_char((char) event->ch) != EXIT_SUCCESS)
      return EXIT_FAILURE;

    if(stroke_process() != EXIT_SUCCESS)
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
