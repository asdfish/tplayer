#define CONFIG_INCLUDE_PLAYLISTS_PATH
#include <config.h>
#include <filesystem.h>
#include <tplayer.h>

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

// private
static int tplayer_check_playlists_path(void) {
  // playlists_path
  if(playlists_path == NULL) {
    printf("Variable \"playlists_path\" is set to NULL.\n");
    return EXIT_FAILURE;
  }
  if(strlen(playlists_path) == 0) {
    printf("Variable \"playlists_path\" has length 0.\n");
    return EXIT_FAILURE;
  }

  struct stat st;
  errno = 0;
  if(stat(playlists_path, &st) != 0) {
    switch(errno) {
      case EACCES:
        printf("Permission denied\n");
        break;
      case EFAULT:
        printf("Bad address\n");
        break;
      case ELOOP:
        printf("Too many symbolic links\n");
        break;
      case ENAMETOOLONG:
        printf("Pathname \"%s\" is too long\n", playlists_path);
        break;
      case ENOENT:
        printf("Pathname \"%s\" does not exist or is a dangling link\n", playlists_path);
        break;
      case ENOMEM:
        printf("Out of kernel memory\n");
        break;
      case ENOTDIR:
        printf("Part of pathname \"%s\" is not a directory", playlists_path);
        break;
      case EOVERFLOW:
        printf("Size of file at \"%s\" is overflowing\n", playlists_path);
        break;
      default:
        printf("An error occured\n");
        break;
    }
    goto exit_failure;
  }

  return EXIT_SUCCESS;

exit_failure:
  return EXIT_FAILURE;
}

typedef int (*tplayer_check_function) (void);
static const tplayer_check_function tplayer_check_functions[] = {
  tplayer_check_playlists_path,
};

static int tplayer_check_config(void) {
  for(unsigned int i = 0; i < ARRAY_LENGTH(tplayer_check_functions); i ++)
    if(tplayer_check_functions[i]() != EXIT_SUCCESS)
      return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

// playlist_pointer - string 2d array storing playlists
// playlist_count   - uint storing count of playlists
// playlist_lengths - uint array storing lengths of each playlist
static int tplayer_get_playlists(const char**** playlists_pointer, unsigned int* playlist_count, unsigned int** playlist_lengths) {
  if(playlists_pointer == NULL || playlist_count == NULL || playlist_lengths == NULL)
    return EXIT_FAILURE;

  const char** playlist_paths = NULL;
  unsigned int playlist_paths_length = 0;

  if(get_dirents_info(playlists_path, DIRENT_IS_DIRECTORY, DIRENT_PATH, &playlist_paths, &playlist_paths_length) != EXIT_SUCCESS)
    return EXIT_FAILURE;
  if(playlist_paths_length == 0) {
    printf("No playlists were found at \"%s\"\n", playlists_path);
    return EXIT_FAILURE;
  }

  *playlists_pointer = NULL;
  *playlists_pointer = (const char***) malloc(playlist_paths_length * sizeof(const char**));
  if(*playlists_pointer == NULL)
    goto free_playlist_paths_contents;
  *playlist_lengths = NULL;
  *playlist_lengths = (unsigned int*) malloc(playlist_paths_length * sizeof(unsigned int));
  if(*playlist_lengths == NULL)
    goto free_playlists_pointer;

  unsigned int i = 0;
  while(i < playlist_paths_length) {
    if(get_dirents_info(*(playlist_paths + i), DIRENT_IS_FILE, DIRENT_PATH, (*playlists_pointer + i), (*playlist_lengths + i)) != EXIT_SUCCESS)
      goto free_playlists_pointer_contents;
    i ++;
  }

  *playlist_count = playlist_paths_length;

  for(unsigned int i = 0; i < playlist_paths_length; i ++) {
    free((char*) playlist_paths[i]);
    playlist_paths[i] = NULL;
  }
  free(playlist_paths);
  playlist_paths = NULL;
  return EXIT_SUCCESS;

free_playlists_pointer_contents:
  for(unsigned int j = 0; j < i; j ++) {
    for(unsigned int k = 0; k < *(*playlist_lengths + j); k ++) {
      free((char*) *(*(*playlists_pointer + j) + k));
      *(*(*playlists_pointer + j) + k) = NULL;
    }

    free(*(*playlists_pointer + j));
    *(*playlists_pointer + j) = NULL;
  }
  free(*playlist_lengths);
  *playlist_lengths = NULL;
free_playlists_pointer:
  free(*playlists_pointer);
  *playlists_pointer = NULL;
free_playlist_paths_contents:
  for(unsigned int i = 0; i < playlist_paths_length; i ++) {
    free((char*) *(playlist_paths + i));
    *(playlist_paths + i) = NULL;
  }
  free(playlist_paths);
  playlist_paths = NULL;
  return EXIT_FAILURE;
}

// public
int tplayer(void) {
  if(tplayer_check_config() != EXIT_SUCCESS)
    return EXIT_FAILURE;

  const char*** playlists = NULL;
  unsigned int playlists_length = 0;
  unsigned int* playlist_lengths = NULL;

  if(tplayer_get_playlists(&playlists, &playlists_length, &playlist_lengths) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  for(unsigned int i = 0; i < playlists_length; i ++) {
    for(unsigned int j = 0; j < playlist_lengths[i]; j ++) {
      printf("%s\n", playlists[i][j]);

      free((char*) playlists[i][j]);
      playlists[i][j] = NULL;
    }
    free(playlists[i]);
    playlists[i] = NULL;
  }
  free(playlists);
  playlists = NULL;

  free(playlist_lengths);
  playlist_lengths = NULL;
  return EXIT_SUCCESS;
}
