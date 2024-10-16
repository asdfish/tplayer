#ifndef GLOBAL_CHECK_H
#define GLOBAL_CHECK_H

#include <global.h>

#ifndef PLAYLISTS_DIRECTORY
#error "PLAYLISTS_DIRECTORY not defined in src/global.h"
#endif

#ifndef MENU_CURSOR_FOREGROUND
#error "MENU_CURSOR_FOREGROUND not defined in src/global.h"
#endif
#ifndef MENU_CURSOR_BACKGROUND
#error "MENU_CURSOR_BACKGROUND not defined in src/global.h"
#endif

#ifndef MENU_NORMAL_FOREGROUND
#error "MENU_NORMAL_FOREGROUND not defined in src/global.h"
#endif
#ifndef MENU_NORMAL_BACKGROUND
#error "MENU_NORMAL_BACKGROUND not defined in src/global.h"
#endif

#ifndef MENU_SPLIT_PERCENT
#error "MENU_SPLIT_PERCENT not defined in src/global.h"
#endif

#ifndef PLAY_METHOD_RANDOM_TEXT
#error "PLAY_METHOD_RANDOM_TEXT not defined in src/global.h"
#endif
#ifndef PLAY_METHOD_ORDER_TEXT
#error "PLAY_METHOD_ORDER_TEXT not defined in src/global.h"
#endif

#ifndef PLAY_METHOD_FOREGROUND
#error "PLAY_METHOD_FOREGROUND not defined in src/global.h"
#endif
#ifndef PLAY_METHOD_BACKGROUND
#error "PLAY_METHOD_BACKGROUND not defined in src/global.h"
#endif

#ifndef PLAY_PERCENTAGE_FOREGROUND
#error "PLAY_PERCENTAGE_FOREGROUND not defined in src/global.h"
#endif
#ifndef PLAY_PERCENTAGE_BACKGROUND
#error "PLAY_PERCENTAGE_BACKGROUND not defined in src/global.h"
#endif

#ifndef KEY_MAP_MENU_MOVE_UP
#error "KEY_MAP_MENU_MOVE_UP not defined in src/global.h"
#endif
#ifndef KEY_MAP_MENU_MOVE_DOWN
#error "KEY_MAP_MENU_MOVE_DOWN not defined in src/global.h"
#endif
#ifndef KEY_MAP_MENU_MOVE_LEFT
#error "KEY_MAP_MENU_MOVE_LEFT not defined in src/global.h"
#endif
#ifndef KEY_MAP_MENU_MOVE_RIGHT
#error "KEY_MAP_MENU_MOVE_RIGHT not defined in src/global.h"
#endif

#ifndef KEY_MAP_MENU_SELECT
#error "KEY_MAP_MENU_SELECT not defined in src/global.h"
#endif

#ifndef KEY_MAP_QUIT
#error "KEY_MAP_QUIT not defined in src/global.h"
#endif

#ifndef KEY_MAP_CHANGE_PLAY_METHOD
#error "KEY_MAP_CHANGE_PLAY_METHOD not defined in src/global.h"
#endif

#ifndef KEY_MAP_SWITCH_SONG
#error "KEY_MAP_SWITCH_SONG not defined in src/global.h"
#endif

#endif
