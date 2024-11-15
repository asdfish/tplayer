#ifndef STATUS_INFO_H
#define STATUS_INFO_H

#include <structs.h>

extern int status_info_init_change_song_function_name(struct StatusInfo* output);
extern int status_info_set_change_song_function_name(struct StatusInfo* output);

extern int status_info_init_current_song_name(struct StatusInfo* output);
extern int status_info_set_current_song_name(struct StatusInfo* output);

extern int status_info_init_current_playlist_name(struct StatusInfo* output);
extern int status_info_set_current_playlist_name(struct StatusInfo* output);

extern int status_info_init_play_percentage(struct StatusInfo* output);
extern int status_info_set_play_percentage(struct StatusInfo* output);

extern int status_info_init_play_percentage_symbol(struct StatusInfo* output);
extern int status_info_set_play_percentage_symbol(struct StatusInfo* output);

#endif
