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

int status_info_init_play_percentage(struct StatusInfo* output) {
  output->contents = (char*) malloc((int_string_length(status_info_play_percentage_max) + 1) * sizeof(char));
  if(output->contents == NULL)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
int status_info_set_play_percentage(struct StatusInfo* output) {
  sprintf(output->contents, "%u", (unsigned int) (status_info_play_percentage_max * audio_play_percentage()));
  return EXIT_SUCCESS;
}
