#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>


#include "replay_data.h"

struct replay_action replay_action_parse(char *replay_action_str) {
    /* Data format: w|x|y|z ending with a comma, where:
     * w: time since last action (can be negative)
     * x: x-coordinate of cursor / player in CTB
     * y: y-coordinate of cursor / 9999 in CTB
     * z: bitwise combo of pressed keys */
    char c = replay_action_str[0];
    uint32_t idx = 0;
    uint32_t x_offset, y_offset, keys_offset;
    for (size_t i = 0; i < 3; ++i) {
        while (c != '|' && c != ',') {
            idx++;
            c = replay_action_str[idx];
        }
        idx++;
        c = replay_action_str[idx];
        switch (i) {
            case 0:
                x_offset = idx;
                break;
            case 1:
                y_offset = idx;
                break;
            case 2:
                keys_offset = idx;
                break;
        }
    }
    int64_t time_delta = atoll(replay_action_str);
    float x = strtof(replay_action_str + x_offset, NULL);
    float y = strtof(replay_action_str + y_offset, NULL);
    uint32_t keys = strtoul(replay_action_str + keys_offset, NULL, 10);
    struct replay_action action = {time_delta, x, y, keys};
    return action;
}

struct replay_data replay_data_parse(char *replay_data_str) {
    size_t replay_size = 0;
    uint64_t idx = 0;
    char c = replay_data_str[idx];
    while (c != '\0') {
        if (c == ',')
            replay_size++;
        idx += 1;
        c = replay_data_str[idx];
    }
    struct replay_data data = {replay_size, malloc(sizeof(struct replay_action) * replay_size)};
    uint64_t action_start = 0, action_end = 0, action_num = 0;
    c = replay_data_str[0];
    while (c != '\0') {
        action_end++;
        if (c == ',') {
            char action[action_end - action_start];
            strncpy(action, replay_data_str + action_start, action_end - action_start);
            data.actions[action_num] = replay_action_parse(action);
            action_start = action_end;
            action_num++;
        }
        c = replay_data_str[action_end];
    }
    free(replay_data_str);
    return data;
}

void replay_data_print(struct replay_data *replay_data) {
    for (size_t i = 0; i < replay_data->size; ++i) {
        printf("%"PRId64": (%f - %f) : %"PRIu32"\n",
               replay_data->actions[i].time_delta,
               replay_data->actions[i].x,
               replay_data->actions[i].y,
               replay_data->actions[i].keys);
    }
}

char* replay_data_to_str(struct replay_data *replay_data) {
    if (replay_data->size == 0)
        return NULL;
    const size_t REPLAY_ACTION_AVERAGE_STR_SIZE = 20;
    size_t total_space = replay_data->size * REPLAY_ACTION_AVERAGE_STR_SIZE;
    size_t used_space = 0;
    const size_t DELTA = 50;
    char *replay_data_str = malloc(total_space);
    for (size_t i = 0; i < replay_data->size; ++i) {
        used_space += sprintf(replay_data_str + used_space, 
                "%"PRId64"|%f|%f|%"PRIu32",", 
                replay_data->actions[i].time_delta, replay_data->actions[i].x,
                replay_data->actions[i].y, replay_data->actions[i].keys);
        if (total_space - used_space < DELTA) {
            total_space *= 2;
            replay_data_str = realloc(replay_data_str, total_space);
        }
    }
    return replay_data_str;
}

void replay_data_free(struct replay_data *replay_data) {
    free(replay_data->actions);
}

