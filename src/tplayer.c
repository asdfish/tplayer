#define CONFIG_INCLUDE_MENU_CONFIGS
#define CONFIG_INCLUDE_PLAYLISTS_PATH
#include <config.h>
#include <filesystem.h>

#include <tb_menu.h>

#include <stdlib.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

static int array_to_menu_items(const char** array, unsigned int array_length, struct TbMenuItem** items);

static void free_playlist_names(void);
static void free_playlist_names_contents(void);

static void free_playlists(void);
static void free_playlists_contents(void);
static void free_playlists_contents_until(unsigned int until);

static void free_playlists_lengths(void);

static void free_playlist_menu_items(void);
static void free_playlists_menus_items(void);
static void free_playlists_menus_items_contents(void);
static void free_playlists_menus_items_contents_until(unsigned int until);

static void free_all(void);
static void free_all_playlists(void);
static void free_all_menu_items(void);

static int init(void);
static int init_menu_items(void);
static int init_playlists(void);

const char** playlist_names = NULL;
unsigned int playlist_names_length = 0;

const char*** playlists = NULL;
unsigned int* playlists_lengths = NULL;

unsigned int selected_playlist = 0;

struct TbMenu playlist_menu;
struct TbMenu* playlists_menus = NULL;

struct TbMenuItem* playlist_menu_items = NULL;
struct TbMenuItem** playlists_menus_items = NULL;

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
    (*items)[i].contents = array[i];
  }

  return EXIT_SUCCESS;
}

static void free_playlist_names(void) {
  free(playlist_names);
  playlist_names = NULL;
}
static void free_playlist_names_contents(void) {
  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    free((char*) playlist_names[i]);
    playlist_names[i] = NULL;
  }
}

static void free_playlists(void) {
  free(playlists);
  playlists = NULL;
}
static void free_playlists_contents(void) {
  free_playlists_contents_until(playlist_names_length);
}
static void free_playlists_contents_until(unsigned int until) {
  for(unsigned int i = 0; i < until; i ++) {
    for(unsigned int j = 0; j < playlists_lengths[i]; j ++) {
      free((char*) playlists[i][j]);
      playlists[i][j] = NULL;
    }
    free(playlists[i]);
    playlists[i] = NULL;
  }
}

static void free_playlists_lengths(void) {
  free(playlists_lengths);
  playlists_lengths = NULL;
}

static void free_playlist_menu_items(void) {
  free(playlist_menu_items);
  playlist_menu_items = NULL;
}
static void free_playlists_menus_items(void) {
  free(playlists_menus_items);
  playlists_menus_items = NULL;
}
static void free_playlists_menus_items_contents(void) {
  free_playlists_menus_items_contents_until(playlist_names_length);
}
static void free_playlists_menus_items_contents_until(unsigned int until) {
  for(unsigned int i = 0; i < until; i ++) {
    free(playlists_menus_items[i]);
    playlists_menus_items[i] = NULL;
  }
}

static void free_all(void) {
  free_all_playlists();
  free_all_menu_items();
}
static void free_all_playlists(void) {
  free_playlists_contents();
  free_playlists();

  free_playlist_names_contents();
  free_playlist_names();

  free_playlists_lengths();
}
static void free_all_menu_items(void) {
  free_playlists_menus_items_contents();
  free_playlists_menus_items();

  free_playlist_menu_items();
}

static int init(void) {
  if(init_playlists() != EXIT_SUCCESS)
    return EXIT_FAILURE;
  if(init_menu_items() != EXIT_SUCCESS)
    goto free_all_playlists;
  return EXIT_SUCCESS;

free_all_playlists:
  free_all_playlists();
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

int tplayer(void) {
  if(init() != EXIT_SUCCESS)
    return EXIT_FAILURE;

  free_all();
  return EXIT_SUCCESS;
}
