#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "lifebar.h"

struct lifebar_tick lifebar_tick_parse(char *lifebar_tick_str) {
    /* Data format: t|v ending with a comma, where:
     * t - time in milliseconds into the song,
     * v - floating point value from 1 to 1 representing amount of life
     * (0 = bar is empty, 1 = bar is full) */
    char c = lifebar_tick_str[0];
    uint32_t idx = 0;
    while (c != '|') {
        idx++;
        c = lifebar_tick_str[idx];
    }
    char* tick_end = lifebar_tick_str + idx;
    uint32_t time = strtoul(lifebar_tick_str, &tick_end, 10);
    double value = strtof(lifebar_tick_str + idx + 1, NULL);
    struct lifebar_tick lt = {time, value};
    return lt;
}

struct lifebar lifebar_parse(char *lifebar_str) {
    if (lifebar_str[0] == '\0') {
        struct lifebar lb = {0, NULL};
        return lb;
    }
    char c = lifebar_str[0];
    size_t size = 1;
    uint64_t idx = 1;
    while (c != '\0') {
        if (c == ',')
            size++;
        c = lifebar_str[idx];
        idx++;
    }
    size -= 1; // ends with an extra comma;
    struct lifebar lifebar = {
        size, malloc(sizeof(struct lifebar_tick) * size)
    };
    uint64_t tick_num = 0, tick_start = 0, tick_end = 0;
    c = lifebar_str[0];
    while (c != '\0') {
        tick_end++;
        if (c == ',') {
            char tick[tick_end - tick_start];
            strncpy(tick, lifebar_str + tick_start, tick_end - tick_start);
            lifebar.ticks[tick_num] = lifebar_tick_parse(tick);
            tick_start = tick_end;
            tick_num++;
        }
        c = lifebar_str[tick_end];
    }
    free(lifebar_str);
    return lifebar;
}

void lifebar_print(struct lifebar *lifebar) {
    printf("Lifebar: ");
    if (lifebar->size == 0){
        printf("none");
    }
    else {
        for (size_t i = 0; i < lifebar->size; ++i) {
            printf("(%" PRIu32 "; %f), ", lifebar->ticks[i].time, lifebar->ticks[i].value);
        }
    }
    printf("\n");
}

char* lifebar_to_str(struct lifebar *lifebar) {
    if (lifebar->size == 0)
        return NULL;
    const size_t LIFEBAR_TICK_STR_AVERAGE_SIZE = 15;
    const size_t DELTA = 50;
    size_t total_space = lifebar->size * LIFEBAR_TICK_STR_AVERAGE_SIZE;
    size_t used_space = 0;
    char *lifebar_str = malloc(total_space);
    for (size_t i = 0; i < lifebar->size; ++i) {
        used_space += sprintf(lifebar_str + used_space, "%"PRIu32"|%f,",
                lifebar->ticks[i].time, lifebar->ticks[i].value);
        if (total_space - used_space < DELTA) {
            total_space *= 2;
            lifebar_str = realloc(lifebar_str, total_space);
        }
    }
    return lifebar_str;
}

void lifebar_free(struct lifebar *lifebar) {
    if (lifebar->size > 0)
        free(lifebar->ticks);
}
