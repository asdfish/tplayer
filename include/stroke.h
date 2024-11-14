#ifndef STROKE_H
#define STROKE_H

#include <structs.h>
#include <stdbool.h>

extern int stroke_add_char(char new_char);
extern unsigned int stroke_bindings_max_length(void);
extern int stroke_process(void);

#endif
