#ifndef STATUS_INFO_H
#define STATUS_INFO_H

#include <stdbool.h>

struct StatusInfoOutput {
  char* contents;
  bool malloced;
};
typedef int (*StatusInfo) (struct StatusInfoOutput* output);

extern int status_info_play_percentage(struct StatusInfoOutput* output);

#endif
