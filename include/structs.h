#ifndef STRUCTS_H
#define STRUCTS_H

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

#endif
