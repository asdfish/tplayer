#include <audio.h>
#include <config.h>
#include <display.h>
#include <init.h>
#include <filesystem.h>
#include <free.h>
#include <event.h>
#include <main.h>
#include <time.h>

static int array_to_menu_items(const char** array, unsigned int array_length, struct TbMenuItem** items);
static int init_audio(void);
static void init_menu(struct TbMenu* menu);
static int init_menus(void);
static int init_menu_items(void);
static int init_playlists(void);
static int init_selections(void);
static int init_strokes(void);

static const char* path_file_name(const char* path);

static int array_to_menu_items(const char** array, unsigned int array_length, struct TbMenuItem** items) {
  if(array == NULL || items == NULL)
    return EXIT_FAILURE;
  if(array_length == 0)
    return EXIT_SUCCESS;

  *items = (struct TbMenuItem*) malloc(array_length * sizeof(struct TbMenuItem));
  if(*items == NULL)
    return EXIT_FAILURE;

  for(unsigned int i = 0; i < array_length; i ++) {
    (*items)[i].foreground = menu_foreground;
    (*items)[i].foreground_reversed = menu_foreground_reversed;
    (*items)[i].contents = path_file_name(array[i]);
  }

  return EXIT_SUCCESS;
}
static void init_menu(struct TbMenu* menu) {
  menu->background = menu_background;
  menu->background_reversed = menu_background_reversed;
}
static const char* path_file_name(const char* path) {
  unsigned int length = strlen(path);
  if(length == 0 || path[length - 1] == '/')
    return path;

  for(int i = length - 1; i >= 0; i --)
    if(path[i] == '/')
      return path + i + 1;
  return path;
}

static int init_audio(void) {
  if(audio_init() != EXIT_SUCCESS)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;

audio_uninit:
  audio_uninit();
  return EXIT_FAILURE;
}
static int init_menus(void) {
  tb_menu_init(&playlist_menu);
  if(tb_menu_set_items(&playlist_menu, playlist_menu_items, playlist_names_length) != TBM_SUCCESS)
    return EXIT_FAILURE;
  init_menu(&playlist_menu);

  playlists_menus = (struct TbMenu*) malloc(playlist_names_length * sizeof(struct TbMenu));
  if(playlists_menus == NULL)
    goto free_playlist_menu;

  unsigned int i = 0;
  while(i < playlist_names_length) {
    tb_menu_init(playlists_menus + i);
    if(tb_menu_set_items(playlists_menus + i, playlists_menus_items[i], playlists_lengths[i]) != TBM_SUCCESS)
      goto free_playlists_menus_contents_until;
    init_menu(playlists_menus + i);
    i ++;
  }

  return EXIT_SUCCESS;

free_playlists_menus_contents_until:
  free_playlists_menus_contents_until(i);
  free_playlists_menus();
free_playlist_menu:
  free_playlist_menu();
  return EXIT_FAILURE;
}
static int init_menu_items(void) {
  if(playlist_names == NULL || playlist_names_length == 0 || playlists == NULL || playlists_lengths == NULL)
    return EXIT_FAILURE;

  if(array_to_menu_items(playlist_names, playlist_names_length, &playlist_menu_items) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  playlists_menus_items = (struct TbMenuItem**) malloc(playlist_names_length * sizeof(struct TbMenuItem*));
  if(playlists_menus_items == NULL)
    goto free_playlist_menu_items;

  unsigned int i = 0;
  while(i < playlist_names_length) {
    if(array_to_menu_items(playlists[i], playlists_lengths[i], playlists_menus_items + i) != EXIT_SUCCESS)
      goto free_menus_items_until;
    i ++;
  }

  return EXIT_SUCCESS;

free_menus_items_until:
  free_playlists_menus_items_contents_until(i);
  free_playlists_menus_items();

free_playlist_menu_items:
  free_playlist_menu_items();
  return EXIT_FAILURE;
}
static int init_playlists(void) {
  if(get_dirents_info(playlists_path, DIRENT_IS_DIRECTORY, DIRENT_PATH, &playlist_names, &playlist_names_length) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  playlists = (const char***) malloc(playlist_names_length * sizeof(const char**));
  if(!playlists)
    goto free_playlist_names;
  playlists_lengths = (unsigned int*) malloc(playlist_names_length * sizeof(unsigned int));
  if(!playlists_lengths)
    goto free_playlists;

  unsigned int i = 0;
  while(i < playlist_names_length) {
    if(get_dirents_info(playlist_names[i], DIRENT_IS_FILE, DIRENT_PATH, playlists + i, playlists_lengths + i) != EXIT_SUCCESS)
      goto free_playlists_contents_until;

    i ++;
  }

  return EXIT_SUCCESS;

free_playlists_contents_until:
  free_playlists_contents_until(i);
  free_playlists_lengths();
free_playlists:
  free_playlists();
free_playlist_names:
  free_playlist_names();
  return EXIT_FAILURE;
}
static int init_selections(void) {
  selected_songs = (unsigned int*) malloc(playlist_names_length * sizeof(unsigned int));
  if(selected_songs == NULL)
    return EXIT_FAILURE;

  display_change_selected_playlist(0);

  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    selected_songs[i] = 1;
    display_change_menu_selection(&selected_songs[i], 0, playlists_menus_items[i], playlists_lengths[i]);
  }

  return EXIT_SUCCESS;
}
static int init_strokes(void) {
  if(o_string_init(&strokes) != O_SUCCESS)
    return EXIT_FAILURE;
  if(o_string_reserve(&strokes, stroke_bindings_max_length()) != O_SUCCESS)
    goto free_all_strokes;
  return EXIT_SUCCESS;

free_all_strokes:
  free_all_strokes();
  return EXIT_FAILURE;
}

int init(void) {
  if(init_playlists() != EXIT_SUCCESS)
    return EXIT_FAILURE;
  if(init_menu_items() != EXIT_SUCCESS)
    goto free_all_playlists;
  if(init_menus() != EXIT_SUCCESS)
    goto free_all_menu_items;
  if(init_strokes() != EXIT_SUCCESS)
    goto free_all_menus;
  if(init_selections() != EXIT_SUCCESS)
    goto free_all_strokes;
  if(init_audio() != EXIT_SUCCESS)
    goto free_all_selections;

  srand(time(NULL));

  return EXIT_SUCCESS;

free_all_selections:
  free_all_selections();
free_all_strokes:
  free_all_strokes();
free_all_menus:
  free_all_menus();
free_all_menu_items:
  free_all_menu_items();
free_all_playlists:
  free_all_playlists();
  return EXIT_FAILURE;
}
