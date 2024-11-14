#include <config.h>
#include <macros.h>
#include <main.h>
#include <stdlib.h>
#include <string.h>

static bool stroke_valid_append(char* concat) {
  return strstr(strokes_allowed_chars, concat) != NULL;
}

int stroke_add_char(char new_char) {
  CHAR_TO_STRING(new_char, string)

  if(stroke_valid_append(string)) {
    if(o_string_cat(&strokes, string) != EXIT_SUCCESS)
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
unsigned int stroke_bindings_max_length(void) {
  unsigned int max_length = 0;
  for(unsigned int i = 0; i < stroke_bindings_length; i ++)
    max_length = MAX(max_length, strlen(stroke_bindings[i].string));

  return max_length;
}
int stroke_process(void) {
  unsigned int max_length = stroke_bindings_max_length();
  unsigned int strokes_length = strlen(strokes.contents);

  bool clear_stroke = false;

  if(strokes_length > max_length) {
    clear_stroke = false;
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
      clear_stroke = true;
  }

clear_stroke:
  if(!clear_stroke) {
    if(o_string_clear(&strokes) != EXIT_SUCCESS)
      return EXIT_FAILURE;
    return EXIT_SUCCESS;
  }
  return EXIT_SUCCESS;
}
