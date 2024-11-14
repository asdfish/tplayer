#ifndef STRUCTS_H
#define STRUCTS_H

struct Argument {
  int i;
};

struct Stroke {
  const char* string;
  int (*function) (const struct Argument*);
  struct Argument argument;
};

#endif
