#include <audio.h>
#include <config.h>
#include <macros.h>
#include <main.h>
#include <status_info.h>
#include <stdlib.h>
#include <stdio.h>

static unsigned int int_string_length(unsigned int input);
static int status_info_init(unsigned int size, struct StatusInfo* info);
static int status_info_set_existing_string(const char* string, struct StatusInfo* info);

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
static int status_info_init(unsigned int size, struct StatusInfo* info) {
  info->contents = (char*) malloc(size * sizeof(char));
  if(info->contents == NULL)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
static int status_info_set_existing_string(const char* string, struct StatusInfo* info) {
  if(strcmp(info->contents, string) != 0)
    strcpy(info->contents, string);
  return EXIT_SUCCESS;
}

int status_info_init_change_song_function_name(struct StatusInfo* output) {
  unsigned int max_length = 0;
  for(unsigned int i = 0; i < change_song_functions_length; i ++)
    max_length = MAX(max_length, strlen(change_song_functions[i].name));
  return status_info_init(max_length, output);
}
int status_info_set_change_song_function_name(struct StatusInfo* output) {
  return status_info_set_existing_string(change_song_functions[selected_change_song_function].name, output);
}

int status_info_init_current_song_name(struct StatusInfo* output) {
  unsigned int max_name_length = 0;
  for(unsigned int i = 0; i < playlist_names_length; i ++)
    for(unsigned int j = 0; j < playlists_lengths[i]; j ++)
      max_name_length = MAX(max_name_length, strlen(playlists_menus_items[i][j].contents));
  return status_info_init(max_name_length, output);
}
int status_info_set_current_song_name(struct StatusInfo* output) {
  return status_info_set_existing_string(playlists_menus_items[selected_playlist][selected_songs[selected_playlist]].contents, output);
}

int status_info_init_current_playlist_name(struct StatusInfo* output) {
  unsigned int max_name_length = 0;
  for(unsigned int i = 0; i < playlist_names_length; i ++)
    max_name_length = MAX(max_name_length, strlen(playlist_menu_items[i].contents));
  return status_info_init(max_name_length, output);
}
int status_info_set_current_playlist_name(struct StatusInfo* output) {
  return status_info_set_existing_string(playlist_menu_items[selected_playlist].contents, output);
}

int status_info_init_play_percentage(struct StatusInfo* output) {
  return status_info_init(int_string_length(status_info_play_percentage_max) + 1, output);
}
int status_info_set_play_percentage(struct StatusInfo* output) {
  sprintf(output->contents, "%u", (unsigned int) (status_info_play_percentage_max * audio_play_percentage()));
  return EXIT_SUCCESS;
}

int status_info_init_play_percentage_symbol(struct StatusInfo* output) {
  unsigned int max_length = 0;
  for(unsigned int i = 0; i < status_info_play_percentage_symbol_length; i ++)
    max_length = MAX(max_length, strlen(status_info_play_percentage_symbol[i]));
  return status_info_init(max_length, output);
}
int status_info_set_play_percentage_symbol(struct StatusInfo* output) {
  unsigned int symbol_index = status_info_play_percentage_symbol_length * audio_play_percentage();
  const char* symbol = status_info_play_percentage_symbol[symbol_index];
  return status_info_set_existing_string(symbol, output);
}

int status_info_init_strokes(struct StatusInfo* output) {
  unsigned int max_length = 0;
  for(unsigned int i = 0; i < stroke_bindings_length; i ++)
    max_length = MAX(max_length, strlen(stroke_bindings[i].string));

  return status_info_init(max_length, output);
}
int status_info_set_strokes(struct StatusInfo* output) {
  return status_info_set_existing_string(strokes.contents, output);
}
