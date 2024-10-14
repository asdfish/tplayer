#ifndef GLOBAL_H
#define GLOBAL_H

extern const char* playlists_directory;

extern const char* menu_selection_prefix;

extern const unsigned int frame_rate;

#define MENU_CURSOR_FOREGROUND TB_BLACK
#define MENU_CURSOR_BACKGROUND TB_WHITE

#define MENU_NORMAL_FOREGROUND TB_WHITE
#define MENU_NORMAL_BACKGROUND TB_BLACK

#define MENU_SPLIT_PERCENT 0.5f

extern unsigned int menu_selection_prefix_length;

extern unsigned int terminal_width, terminal_height;

extern void global_set_terminal_size(void);
extern void global_init(void);

#endif
