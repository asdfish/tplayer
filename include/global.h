#ifndef GLOBAL_H
#define GLOBAL_H

#define PLAYLISTS_DIRECTORY "/home/andre/files/music"

extern const char* menu_selection_prefix;

extern const unsigned int frame_rate;

#define MENU_CURSOR_FOREGROUND TB_BLACK
#define MENU_CURSOR_BACKGROUND TB_WHITE

#define MENU_NORMAL_FOREGROUND TB_WHITE
#define MENU_NORMAL_BACKGROUND TB_BLACK

#define MENU_SPLIT_PERCENT 0.5f

#define PLAY_METHOD_RANDOM_TEXT " random |"
#define PLAY_METHOD_ORDER_TEXT  " order  |"

#define PLAY_METHOD_FOREGROUND TB_WHITE
#define PLAY_METHOD_BACKGROUND TB_BLACK

#define PLAY_PERCENTAGE_FOREGROUND TB_WHITE
#define PLAY_PERCENTAGE_BACKGROUND TB_BLACK

#define KEY_MAP_MENU_MOVE_UP 'k'
#define KEY_MAP_MENU_MOVE_DOWN 'j'
#define KEY_MAP_MENU_MOVE_LEFT 'h'
#define KEY_MAP_MENU_MOVE_RIGHT 'l'

#define KEY_MAP_MENU_SELECT ' '

#define KEY_MAP_QUIT 'q'

#define KEY_MAP_CHANGE_PLAY_METHOD 'p'

#define KEY_MAP_SWITCH_SONG 's'

extern unsigned int menu_selection_prefix_length;

extern unsigned int terminal_width, terminal_height;

extern unsigned int play_method;

extern void global_change_play_method(void);
extern void global_set_terminal_size(void);
extern void global_set_menu_selection_prefix_length(void);

#endif
