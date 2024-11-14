#define CONFIG_INCLUDE_MENU_CONFIGS
#define CONFIG_INCLUDE_PLAYLISTS_PATH
#include <config.h>
#include <init.h>
#include <filesystem.h>
#include <free.h>
#include <path.h>
#include <tplayer.h>

#include <tb_menu.h>

static int array_to_menu_items(const char** array, unsigned int array_length, struct TbMenuItem** items);

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
    (*items)[i].contents = path_get_file_name(array[i]);
  }

  return EXIT_SUCCESS;
}

int init(void) {
  if(init_tplayer_playlists() != EXIT_SUCCESS)
    return EXIT_FAILURE;
  if(init_tplayer_menu_items() != EXIT_SUCCESS)
    goto free_all_tplayer_playlists;
  if(init_tplayer_menus() != EXIT_SUCCESS)
    goto free_all_menu_items;
  return EXIT_SUCCESS;

free_all_menu_items:
  free_all_menu_items();
free_all_tplayer_playlists:
  free_all_tplayer_playlists();
  return EXIT_FAILURE;
}
int init_tplayer_menus(void) {
  tb_menu_init(&tplayer_playlist_menu);
  if(tb_menu_set_items(&tplayer_playlist_menu, tplayer_playlist_menu_items, tplayer_playlist_names_length) != TBM_SUCCESS)
    return EXIT_FAILURE;

  tplayer_playlists_menus = (struct TbMenu*) malloc(tplayer_playlist_names_length * sizeof(struct TbMenu));
  if(tplayer_playlists_menus == NULL)
    goto free_tplayer_playlist_menu;

  unsigned int i = 0;
  while(i < tplayer_playlist_names_length) {
    tb_menu_init(tplayer_playlists_menus + i);
    if(tb_menu_set_items(tplayer_playlists_menus + i, tplayer_playlists_menus_items[i], tplayer_playlists_lengths[i]) != TBM_SUCCESS)
      goto free_tplayer_playlists_menus_contents_until;
    i ++;
  }

  return EXIT_SUCCESS;

free_tplayer_playlists_menus_contents_until:
  free_tplayer_playlists_menus_contents_until(i);
  free_tplayer_playlists_menus();
free_tplayer_playlist_menu:
  free_tplayer_playlist_menu();
  return EXIT_FAILURE;
}
int init_tplayer_menu_items(void) {
  if(tplayer_playlist_names == NULL || tplayer_playlist_names_length == 0 || tplayer_playlists == NULL || tplayer_playlists_lengths == NULL)
    return EXIT_FAILURE;

  if(array_to_menu_items(tplayer_playlist_names, tplayer_playlist_names_length, &tplayer_playlist_menu_items) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  tplayer_playlists_menus_items = (struct TbMenuItem**) malloc(tplayer_playlist_names_length * sizeof(struct TbMenuItem*));
  if(tplayer_playlists_menus_items == NULL)
    goto free_tplayer_playlist_menu_items;

  unsigned int i = 0;
  while(i < tplayer_playlist_names_length) {
    if(array_to_menu_items(tplayer_playlists[i], tplayer_playlists_lengths[i], tplayer_playlists_menus_items + i) != EXIT_SUCCESS)
      goto free_menus_items_until;
    i ++;
  }

  return EXIT_SUCCESS;

free_menus_items_until:
  free_tplayer_playlists_menus_items_contents_until(i);
  free_tplayer_playlists_menus_items();

free_tplayer_playlist_menu_items:
  free_tplayer_playlist_menu_items();
  return EXIT_FAILURE;
}
int init_tplayer_playlists(void) {
  if(get_dirents_info(playlists_path, DIRENT_IS_DIRECTORY, DIRENT_PATH, &tplayer_playlist_names, &tplayer_playlist_names_length) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  tplayer_playlists = (const char***) malloc(tplayer_playlist_names_length * sizeof(const char**));
  if(!tplayer_playlists)
    goto free_tplayer_playlist_names;
  tplayer_playlists_lengths = (unsigned int*) malloc(tplayer_playlist_names_length * sizeof(unsigned int));
  if(!tplayer_playlists_lengths)
    goto free_tplayer_playlists;

  unsigned int i = 0;
  while(i < tplayer_playlist_names_length) {
    if(get_dirents_info(tplayer_playlist_names[i], DIRENT_IS_FILE, DIRENT_PATH, tplayer_playlists + i, tplayer_playlists_lengths + i) != EXIT_SUCCESS)
      goto free_tplayer_playlists_contents_until;

    i ++;
  }

  return EXIT_SUCCESS;

free_tplayer_playlists_contents_until:
  free_tplayer_playlists_contents_until(i);
  free_tplayer_playlists_lengths();
free_tplayer_playlists:
  free_tplayer_playlists();
free_tplayer_playlist_names:
  free_tplayer_playlist_names();
  return EXIT_FAILURE;
}
