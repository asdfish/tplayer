#define CONFIG_INCLUDE_PLAYLISTS_PATH
#include <config.h>
#include <filesystem.h>

#include <stdlib.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

static void free_playlist_names(void);
static void free_playlist_names_contents(void);

static void free_playlists(void);
static void free_playlists_contents(void);
static void free_playlists_contents_until(unsigned int until);

static void free_playlists_lengths(void);

static void free_all(void);

static int init(void);

const char** playlist_names = NULL;
unsigned int playlist_names_length = 0;

const char*** playlists = NULL;
unsigned int* playlists_lengths = NULL;

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

static void free_all(void) {
  free_playlists_contents();
  free_playlists();

  free_playlist_names_contents();
  free_playlist_names();

  free_playlists_lengths();
}

static int init(void) {
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

