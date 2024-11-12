#define CONFIG_INCLUDE_MENU_CONFIGS
#define CONFIG_INCLUDE_PLAYLISTS_PATH
#include <config.h>
#include <filesystem.h>
#include <path.h>
#include <tplayer.h>

#include <tb_menu.h>

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

// private
static inline int array_to_menu_items(const char** array, unsigned int array_length, struct TbMenuItem** output) {
  if(array == NULL || output == NULL)
    return EXIT_FAILURE;

  *output = (struct TbMenuItem*) malloc(array_length * sizeof(struct TbMenuItem));
  if(*output == NULL)
    return EXIT_FAILURE;

  for(unsigned int i = 0; i < array_length; i ++) {
    (*output)[i].foreground = menu_foreground;
    (*output)[i].foreground_reversed = menu_foreground_reversed;
    (*output)[i].contents = path_file_name(array[i]);
  }

  return EXIT_SUCCESS;
}

static inline int check_playlists_path(void) {
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

typedef int (*check_function) (void);
static const check_function check_functions[] = {
  check_playlists_path,
};

static inline int check_config(void) {
  for(unsigned int i = 0; i < ARRAY_LENGTH(check_functions); i ++)
    if(check_functions[i]() != EXIT_SUCCESS)
      return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

static inline void free_playlists(const char** playlist_names, unsigned int playlist_names_length, const char*** playlists, unsigned int* playlist_lengths) {
  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    for(unsigned int j = 0; j < playlist_lengths[i]; j ++) {
      free((char*) playlists[i][j]);
      playlists[i][j] = NULL;
    }
    free(playlists[i]);
    playlists[i] = NULL;

    free((char*) playlist_names[i]);
    playlist_names[i] = NULL;
  }
  free(playlist_lengths);
  playlist_lengths = NULL;
  free(playlist_names);
  playlist_names = NULL;
  free(playlists);
  playlists = NULL;
}

static inline int get_playlists(const char*** playlist_names, unsigned int* playlist_names_length,const char**** playlists, unsigned int** playlist_lengths) {
  if(playlist_names == NULL || playlist_names_length == NULL ||
      playlists == NULL || playlist_lengths == NULL)
    return EXIT_FAILURE;

  const char** playlist_paths = NULL;
  unsigned int playlist_paths_length = 0;

  if(get_dirents_info(playlists_path, DIRENT_IS_DIRECTORY, DIRENT_PATH, &playlist_paths, &playlist_paths_length) != EXIT_SUCCESS)
    return EXIT_FAILURE;
  if(playlist_paths_length == 0) {
    printf("No playlists were found at \"%s\"\n", playlists_path);
    return EXIT_FAILURE;
  }

  *playlists = NULL;
  *playlists = (const char***) malloc(playlist_paths_length * sizeof(const char**));
  if(*playlists == NULL)
    goto free_playlist_paths_contents;
  *playlist_lengths = NULL;
  *playlist_lengths = (unsigned int*) malloc(playlist_paths_length * sizeof(unsigned int));
  if(*playlist_lengths == NULL)
    goto free_playlists;

  unsigned int i = 0;
  while(i < playlist_paths_length) {
    if(get_dirents_info(playlist_paths[i], DIRENT_IS_FILE, DIRENT_PATH, (*playlists + i), (*playlist_lengths + i)) != EXIT_SUCCESS)
      goto free_playlists_contents;
    i ++;
  }

  *playlist_names = playlist_paths;
  *playlist_names_length = playlist_paths_length;
  return EXIT_SUCCESS;

free_playlists_contents:
  for(unsigned int j = 0; j < i; j ++) {
    for(unsigned int k = 0; k < (*playlist_lengths)[j]; k ++) {
      free((char*) (*playlists)[j][k]);
      (*playlists)[j][k] = NULL;
    }

    free((*playlists)[j]);
    (*playlists)[j] = NULL;
  }
  free(*playlist_lengths);
  *playlist_lengths = NULL;
free_playlists:
  free(*playlists);
  *playlists = NULL;
free_playlist_paths_contents:
  for(unsigned int i = 0; i < playlist_paths_length; i ++) {
    free((char*) playlist_paths[i]);
    playlist_paths[i] = NULL;
  }
  free(playlist_paths);
  playlist_paths = NULL;
  return EXIT_FAILURE;
}

static int resize_menus(struct TbMenu* playlists_menu, struct TbMenu** playlist_menus, unsigned int playlist_menus_length) {
  unsigned int original_width = tb_width();
  unsigned int playlists_menu_width = original_width * menu_split;
  unsigned int playlist_menus_width = original_width - playlists_menu_width;
  
  unsigned int height = tb_height();

  playlists_menu->width = playlists_menu_width;
  playlists_menu->height = height;
  for(unsigned int i = 0; i < playlist_menus_length; i ++) {
    (*playlist_menus)[i].x =  1;
    (*playlist_menus)[i].width = 69;
    (*playlist_menus)[i].height = height;
  }

  return EXIT_SUCCESS;
}

// public
int tplayer(void) {
  int exit_code = EXIT_SUCCESS;

  if(check_config() != EXIT_SUCCESS) {
    exit_code = EXIT_FAILURE;
    goto exit;
  }

  const char** playlist_names = NULL;
  unsigned int playlist_names_length = 0;

  const char*** playlists = NULL;
  unsigned int* playlist_lengths = NULL;

  if(get_playlists(&playlist_names, &playlist_names_length, &playlists, &playlist_lengths) != EXIT_SUCCESS) {
    exit_code = EXIT_FAILURE;
    goto exit;
  }

  struct TbMenuItem* playlist_menu_items = NULL;
  if(array_to_menu_items(playlist_names, playlist_names_length, &playlist_menu_items) != EXIT_SUCCESS) {
    exit_code = EXIT_FAILURE;
    goto free_playlists;
  }

  struct TbMenuItem** playlist_menus_items = NULL;
  if(array_2d_to_menu_items(playlists, playlist_lengths, playlist_names_length, &playlist_menus_items) != EXIT_SUCCESS) {
    exit_code = EXIT_FAILURE;
    goto free_playlist_menu_items;
  }

  struct TbMenu playlist_menu;
  playlist_menu.background = menu_background;
  playlist_menu.background_reversed = menu_background_reversed;
  playlist_menu.enable_reversed_colors = true;
  tb_menu_init(&playlist_menu);
  if(tb_menu_set_items(&playlist_menu, playlist_menu_items, playlist_names_length) != TBM_SUCCESS)
    goto free_playlist_menus_items_contents;

  struct TbMenu* playlist_menus = (struct TbMenu*) malloc(playlist_names_length * sizeof(struct TbMenu));
  if(playlist_menus == NULL)
    goto tb_menu_uninit_playlist_menu;
  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    playlist_menus[i].background = menu_background;
    playlist_menus[i].background_reversed = menu_background_reversed;
    playlist_menus[i].enable_reversed_colors = true;
    tb_menu_init(playlist_menus + i);
    tb_menu_set_items(playlist_menus + i, playlist_menus_items[i], playlist_lengths[i]);
  }

  tb_init();

  resize_menus(&playlist_menu, &playlist_menus, playlist_names_length);

  int selected_playlist = 0;

  while(1) {
    /*struct TbMenu* menus[] = {*/
    /*  &playlist_menu,*/
    /*  &playlist_menus[0]*/
    /*};*/
    tb_menu_draw(&playlist_menus[0]);
    tb_present();

    struct tb_event event;
    tb_poll_event(&event);
    if(event.ch == 'q')
      break;
  }

  tb_shutdown();

  free(playlist_menus);
  playlist_menus = NULL;
tb_menu_uninit_playlist_menu:
  tb_menu_uninit(&playlist_menu);
free_playlist_menus_items_contents:
  for(unsigned int i = 0; i < playlist_names_length; i ++) {
    free(playlist_menus_items[i]);
    playlist_menus_items[i] = NULL;
  }
  free(playlist_menus_items);
  playlist_menus_items = NULL;
free_playlist_menu_items:
  free(playlist_menu_items);
  playlist_menu_items = NULL;
free_playlists:
  free_playlists(playlist_names, playlist_names_length, playlists, playlist_lengths);
exit:
  return exit_code;
}
