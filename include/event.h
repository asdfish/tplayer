#ifndef EVENT_H
#define EVENT_H

#include <termbox2.h>

extern int event_handle(struct tb_event* event);
extern unsigned int stroke_bindings_max_length(void);

#endif
