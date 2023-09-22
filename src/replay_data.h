#ifndef REPLAY_DATA_H
#define REPLAY_DATA_H

#include <stdlib.h>
#include <stdint.h>

struct replay_action {
    int64_t time_delta;
    float x;
    float y;
    uint32_t keys;
};

struct replay_data {
    size_t size;
    struct replay_action *actions;
};

struct replay_action replay_action_parse(char *replay_action_str);
struct replay_data replay_data_parse(char *replay_data_str);
void replay_data_print(struct replay_data *replay_data);
void replay_data_free(struct replay_data *replay_data);
char* replay_data_to_str(struct replay_data *replay_data);

#endif
