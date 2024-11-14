#ifndef MACROS_H
#define MACROS_H

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#define CHAR_TO_STRING(character, string_name) \
  char string_name[2]; \
  string_name[0] = character; \
  string_name[1] = '\0';

#define GET_MENUS(array_name) \
struct TbMenu* array_name[2] = { \
  &playlist_menu, \
  playlists_menus + selected_playlist, \
};

#endif
