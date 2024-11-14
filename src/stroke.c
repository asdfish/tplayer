#define CONFIG_INCLUDE_STROKE_CONFIGS
#include <config.h>
#include <macros.h>
#include <main.h>
#include <stdlib.h>
#include <string.h>

static bool stroke_valid_append(char* concat) {
  return strstr(concat, strokes_allowed_chars) != NULL;
}

int stroke_add_char(char new_char) {
  CHAR_TO_STRING(new_char, string)

  if(stroke_valid_append(string))
    if(o_string_cat(&strokes, string) != EXIT_SUCCESS)
      return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
unsigned int stroke_bindings_max_length(void) {
  unsigned int max_length = 0;
  for(unsigned int i = 0; i < ARRAY_LENGTH(stroke_bindings); i ++)
    max_length = MAX(max_length, strlen(stroke_bindings[i].string));

  return max_length;
}
int stroke_execute(void) {
  return EXIT_SUCCESS;
}
