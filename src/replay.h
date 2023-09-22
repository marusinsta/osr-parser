#ifndef REPLAY_H
#define REPLAY_H
#include <inttypes.h>

#include "lifebar.h"
#include "replay_data.h"

enum GameMode {
    STD = 0,
    TAIKO = 1,
    CTB = 2,
    MANIA = 3,
};

struct replay {
    enum GameMode mode;
    uint32_t version; 
    char *beatmap_hash;
    char *player_name;
    char *replay_hash;
    uint16_t acc_300, acc_100, acc_50, acc_geki, acc_katu, acc_miss;
    uint32_t score;
    uint16_t max_combo;
    uint8_t perfect;
    uint32_t mods;
    struct lifebar lifebar;
    uint64_t timestamp;
    struct replay_data replay_data;
    uint64_t score_id;
    double additional_info;
};

void replay_print(struct replay *replay);
void replay_free(struct replay *replay);

#endif
