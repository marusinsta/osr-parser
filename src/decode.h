#ifndef DECODE_H
#define DECODE_H

#include <stdbool.h>

bool check_if_osr(char *path);
struct replay replay_parse(char *path);

#endif
