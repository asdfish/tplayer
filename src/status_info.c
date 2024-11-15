#include <audio.h>
#include <config.h>
#include <status_info.h>
#include <stdlib.h>
#include <stdio.h>

static unsigned int int_string_length(unsigned int input) {
  if(input == 0)
    return 1;

  unsigned int length = 0;
  while(input > 0) {
    input /= 10;
    length ++;
  }

  return length;
}

int status_info_play_percentage(struct StatusInfoOutput* output) {
  unsigned int play_percentage = status_info_play_percentage_max * audio_play_percentage();
  unsigned int string_length = int_string_length(play_percentage) + 1;

  output->contents = (char*) malloc(string_length * sizeof(char));
  if(output->contents == NULL)
    return EXIT_FAILURE;

  if(snprintf(output->contents, string_length, "%u", play_percentage) < 0)
    return EXIT_FAILURE;
  output->malloced = true;
  return EXIT_SUCCESS;
}
