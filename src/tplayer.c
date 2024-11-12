#define CONFIG_INCLUDE_PLAYLISTS_PATH
#include <config.h>
#include <tplayer.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

// private
int tplayer_check_playlists_path(void) {
  // playlists_path
  if(playlists_path == NULL) {
    printf("Variable \"playlists_path\" is set to NULL.\n");
    return -1;
  }
  if(strlen(playlists_path) == 0) {
    printf("Variable \"playlists_path\" has length 0.\n");
    return -1;
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

  return 0;

exit_failure:
  return -1;
}

typedef int (*tplayer_check_function) (void);
static const tplayer_check_function tplayer_check_functions[] = {
  tplayer_check_playlists_path,
};

static int tplayer_check_config(void) {
  for(unsigned int i = 0; i < ARRAY_LENGTH(tplayer_check_functions); i ++)
    if(tplayer_check_functions[i]() != 0)
      return -1;
  return 0;
}

// playlist_pointer - string 2d array storing playlists
// playlist_count   - uint storing count of playlists
// playlist_lengths - uint array storing lengths of each playlist
int tplayer_get_playlists(const char**** playlist_pointer, unsigned int* playlist_count, unsigned int** playlist_lengths) {
  if(playlist_pointer == NULL || playlist_count == NULL || playlist_lengths == NULL)
    return -1;

  return 0;
}

// public
int tplayer(void) {
  if(tplayer_check_config() != 0)
    return -1;
  return 0;
}
