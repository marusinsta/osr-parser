#ifndef LIFEBAR_H
#define LIFEBAR_H

#include "inttypes.h"
#include <stdlib.h>

struct lifebar_tick {
    uint32_t time;
    float value;
};

struct lifebar {
    size_t size;
    struct lifebar_tick *ticks;
};

struct lifebar lifebar_parse(char *lifebar_str);
void lifebar_print(struct lifebar *lifebar);
void lifebar_free(struct lifebar *lifebar);
char* lifebar_to_str(struct lifebar *lifebar);

#endif
