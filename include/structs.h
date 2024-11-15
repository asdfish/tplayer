#ifndef STRUCTS_H
#define STRUCTS_H

#include <change_song.h>
#include <stdbool.h>
#include <stdint.h>

struct Argument {
  int i;
};

struct KeyBinding {
  uint16_t key; // TB_KEY_*
  int (*function) (const struct Argument*);
  struct Argument argument;
};

struct StrokeBinding {
  const char* string;
  int (*function) (const struct Argument*);
  struct Argument argument;
};

struct ChangeSongFunctionConfig {
  ChangeSongFunction function;
  const char* name;
};

struct StatusInfo;

typedef int (*StatusInfoFunction) (struct StatusInfo* output);
struct StatusInfo {
  StatusInfoFunction init_function;
  StatusInfoFunction set_function;
  char* contents;
};

#endif
