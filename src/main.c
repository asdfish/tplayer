#include <audio.h>
#include <directory.h>
#include <global.h>
#include <global_check.h>
#include <menu.h>

#include <termbox2.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int exit_code = -1;

void draw_menus(struct Menu* playlist_menu, struct Menu* song_menus, bool cursor_x);
void draw_play_method(void);
void draw_play_percentage(void);
void resize_menus(struct Menu* playlist_menu, struct Menu* song_menus, unsigned int song_menus_length);

int main(void) {
  // allocating
  const char** playlist_paths;
  unsigned int playlist_paths_length;
  if(directory_directory_paths(PLAYLISTS_DIRECTORY, &playlist_paths, &playlist_paths_length) != 0) {
    fprintf(stderr, "Failed to get the path of directories in %s\n", PLAYLISTS_DIRECTORY);
    goto exit;
  }
  const char** playlist_names;
  unsigned int playlist_names_length;
  if(directory_directory_names(PLAYLISTS_DIRECTORY, &playlist_names, &playlist_names_length) != 0) {
    fprintf(stderr, "Failed to get the name of directories in %s\n", PLAYLISTS_DIRECTORY);
    goto free_playlist_paths;
  }

  if(playlist_paths_length != playlist_names_length) {
    fprintf(stderr, "Got different lengths for playlist paths (%u) and playlist names (%u)\n", playlist_paths_length, playlist_names_length);
    goto free_playlist_names;
  }

  const char*** song_paths = (const char***) malloc(playlist_paths_length * sizeof(const char**));
  if(song_paths == NULL) {
    fprintf(stderr, "Malloc failure\n");
    goto free_playlist_names;
  }
  unsigned int* song_path_lengths = (unsigned int*) malloc(playlist_paths_length * sizeof(unsigned int));
  if(song_path_lengths == NULL) {
    fprintf(stderr, "Malloc failure\n");
    goto free_song_paths;
  }

  unsigned int song_paths_end = playlist_paths_length;
  for(unsigned int i = 0; i < playlist_paths_length; i ++)
    if(directory_file_paths(playlist_paths[i], &song_paths[i], &song_path_lengths[i]) != 0) {
      fprintf(stderr, "Failed to get the path of files in %s\n", playlist_paths[i]);
      song_paths_end = i;
      goto free_song_paths_contents;
    }

  const char*** song_names = (const char***) malloc(playlist_paths_length * sizeof(const char**));
  if(song_names == NULL) {
    fprintf(stderr, "Malloc failure\n");
    goto free_song_paths_contents;
  }
  unsigned int* song_name_lengths = (unsigned int*) malloc(playlist_names_length * sizeof(unsigned int));
  if(song_name_lengths == NULL) {
    fprintf(stderr, "Malloc failure\n");
    goto free_song_names;
  }

  unsigned int song_names_end = playlist_paths_length;
  for(unsigned int i = 0; i < playlist_names_length; i ++)
    if(directory_file_names(playlist_paths[i], &song_names[i], &song_name_lengths[i]) != 0) {
      fprintf(stderr, "Failed to get the name of files in %s\n", playlist_paths[i]);
      song_names_end = i;
      goto free_song_names_contents;
    }

  {
    bool consistent_lengths = true;
    for(unsigned int i = 0; i < playlist_names_length; i ++)
      if(song_path_lengths[i] != song_name_lengths[i]) {
        consistent_lengths = false;
        break;
      }

    if(!consistent_lengths) {
      fprintf(stderr, "Got different amount of songs when getting song names and paths\n");
      goto free_song_names_contents;
    }
  }

  struct Menu playlist_menu = {
    0, 1, 0, 0,
    0, 0,
    true, 0,
    NULL, NULL, 0,
  };
  if(menu_change_items(&playlist_menu, playlist_names, playlist_names_length) != 0) {
    fprintf(stderr, "Malloc failure\n");
    goto free_song_names_contents;
  }

  struct Menu* song_menus = (struct Menu*) malloc(playlist_paths_length * sizeof(struct Menu));
  if(song_menus == NULL) {
    fprintf(stderr, "Malloc failure\n");
    goto free_playlist_menu;
  }

  unsigned int song_menus_end = playlist_paths_length;
  for(unsigned int i = 0; i < playlist_paths_length; i ++) {
    song_menus[i].x = 0;
    song_menus[i].y = 1;
    song_menus[i].enable_selections = true;
    song_menus[i].selection = 0;

    if(menu_change_items(&song_menus[i], song_names[i], song_name_lengths[i]) != 0) {
      song_menus_end = i;
      goto free_song_menus_contents;
    }
  }

  global_set_menu_selection_prefix_length();

  if(audio_init() != 0) {
    fprintf(stderr, "Soloud\n");
    goto free_song_menus_contents;
  }

  tb_init();

  resize_menus(&playlist_menu, song_menus, playlist_paths_length);

  bool switch_song = true;
  bool restart_song = true;
  bool menu = false;

  bool redraw_menus = true;
  bool redraw_play_method = true;
  bool redraw_play_percentage = true;

  srand(time(NULL));
  while(true) {
    if(!audio_is_playing() || switch_song) {
      if(play_method == 0) {
        song_menus[playlist_menu.selection].selection = rand() % song_path_lengths[playlist_menu.selection];
      } else if(play_method == 1) {
        song_menus[playlist_menu.selection].selection ++;
        if(song_menus[playlist_menu.selection].selection >= song_path_lengths[playlist_menu.selection])
          song_menus[playlist_menu.selection].selection = 0;
      }
      switch_song = false;
      restart_song = true;
    }

    if(restart_song) {
      if(audio_play(song_paths[playlist_menu.selection][song_menus[playlist_menu.selection].selection]) != 0) {
        tb_shutdown();
        fprintf(stderr, "Failed to play %s\n", song_paths[playlist_menu.selection][song_menus[playlist_menu.selection].selection]);
        goto audio_uninit;
      }
      restart_song = false;
      redraw_menus = true;
    }

    if(redraw_menus || redraw_play_method || redraw_play_percentage) {
      if(redraw_menus) {
        draw_menus(&playlist_menu, song_menus, menu);
        redraw_menus = false;
      }
      if(redraw_play_method) {
        draw_play_method();
        redraw_play_method = false;
      }
      if(redraw_play_percentage) {
        draw_play_percentage();
        redraw_play_percentage = false;
      }

      tb_present();
    }

    struct tb_event event;
    tb_peek_event(&event, frame_rate);

    if(event.type == TB_EVENT_RESIZE) {
      resize_menus(&playlist_menu, song_menus, playlist_paths_length);
      redraw_menus = true;
      redraw_play_method = true;
      redraw_play_percentage = true;
      continue;
    }

    switch(event.ch) {
      case KEY_MAP_QUIT:
        goto quit;
      case KEY_MAP_MENU_MOVE_LEFT:
        if(menu == 1) {
          menu = 0;
          redraw_menus = true;
        }
        continue;
      case KEY_MAP_MENU_MOVE_RIGHT:
        if(menu == 0) {
          menu = 1;
          redraw_menus = true;
        }
        continue;
      case KEY_MAP_MENU_MOVE_DOWN:
        if(menu == 0)
          menu_move_cursor(&playlist_menu, 1);
        else
          menu_move_cursor(&song_menus[playlist_menu.selection], 1);
        redraw_menus = true;
        continue;
      case KEY_MAP_MENU_MOVE_UP:
        if(menu == 0)
          menu_move_cursor(&playlist_menu, -1);
        else
          menu_move_cursor(&song_menus[playlist_menu.selection], -1);
        redraw_menus = true;
        continue;
      case KEY_MAP_MENU_SELECT:
        if(menu == 0)
          menu_select(&playlist_menu);
        else {
          menu_select(&song_menus[playlist_menu.selection]);
          restart_song = true;
        }
        redraw_menus = true;
        continue;
      case KEY_MAP_CHANGE_PLAY_METHOD:
        global_change_play_method();
        redraw_play_method = true;
        continue;
      case KEY_MAP_SWITCH_SONG:
        switch_song = true;
        redraw_menus = true;
        continue;
    }

    redraw_play_percentage = true;
  }

quit:
  exit_code = 0;
  // deallocating
  tb_shutdown();
audio_uninit:
  audio_uninit();
free_song_menus_contents:
  for(unsigned int i = 0; i < song_menus_end; i ++)
    menu_free(&song_menus[i]);
  free(song_menus);
  song_menus = NULL;
free_playlist_menu:
  menu_free(&playlist_menu);
free_song_names_contents:
  for(unsigned int i = 0; i < song_names_end; i ++) {
    for(unsigned int j = 0; j < song_name_lengths[i]; j ++) {
      free((char*) song_names[i][j]);
      song_names[i][j] = NULL;
    }
    free(song_names[i]);
    song_names[i] = NULL;
  }
  free(song_name_lengths);
  song_name_lengths = NULL;
free_song_names:
  free(song_names);
  song_names = NULL;
free_song_paths_contents:
  for(unsigned int i = 0; i < song_paths_end; i ++) {
    for(unsigned int j = 0; j < song_path_lengths[i]; j ++) {
      free((char*) song_paths[i][j]);
      song_paths[i][j] = NULL;
    }
    free(song_paths[i]);
    song_paths[i] = NULL;
  }
  free(song_path_lengths);
  song_path_lengths = NULL;
free_song_paths:
  free(song_paths);
  song_paths = NULL;
free_playlist_names:
  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    free((char*) playlist_names[i]);
    playlist_names[i] = NULL;
  }
  free(playlist_names);
  playlist_names = NULL;
free_playlist_paths:
  for(unsigned int i = 0; i < playlist_paths_length; i ++) {
    free((char*) playlist_paths[i]);
    playlist_paths[i] = NULL;
  }
  free(playlist_paths);
  playlist_paths = NULL;
exit:
  if(exit_code != 0)
    fflush(stderr);
  return exit_code;
}

void draw_menus(struct Menu* playlist_menu, struct Menu* song_menus, bool cursor_x) {
  menu_draw(playlist_menu, !cursor_x);
  menu_draw(&song_menus[playlist_menu->selection], cursor_x);
}

void draw_play_method(void) {
  char* play_method_text = NULL;
  switch(play_method) {
    case 0:
      play_method_text = PLAY_METHOD_RANDOM_TEXT;
      break;
    case 1:
      play_method_text = PLAY_METHOD_ORDER_TEXT;
      break;
  }

  tb_print(0, 0, PLAY_METHOD_FOREGROUND, PLAY_METHOD_BACKGROUND, play_method_text);
}

void draw_play_percentage(void) {
  float play_percentage = audio_play_percentage();
  unsigned int play_percentage_width = (unsigned int) ((float) terminal_width * play_percentage);

  for(unsigned int i = 0; i < terminal_width; i ++)
    tb_set_cell(i, terminal_height - 1, i < play_percentage_width ? '=' : ' ', PLAY_PERCENTAGE_FOREGROUND, PLAY_PERCENTAGE_BACKGROUND);
}

void resize_menus(struct Menu* playlist_menu, struct Menu* song_menus, unsigned int song_menus_length) {
  global_set_terminal_size();
  playlist_menu->width = terminal_width * MENU_SPLIT_PERCENT;
  playlist_menu->height = terminal_height - 2;

  unsigned int song_menus_width = terminal_width - playlist_menu->width;

  for(unsigned int i = 0; i < song_menus_length; i ++) {
    song_menus[i].x = playlist_menu->width;
    song_menus[i].width = song_menus_width;
    song_menus[i].height = terminal_height - 2;
  }
}
