#include "mods.h"
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 256

struct used_mods mods_parse_from_int(int32_t mods_int) {
    struct used_mods um;
    um.size = 0;
    for (int i = 0; i < MOD_COUNT; ++i) {
        if ((mods_int & 1 << i) != 0) {
            um.mods[um.size] = i;
            um.size++;
        }
    }
    return um;
}

char* mods_to_full_string(struct used_mods *um){
    char *str = malloc(sizeof(char) * STRING_SIZE);
    strcpy(str, "");
    for (int i = 0; i < um->size; ++i) {
        strcat(str, MODS_INFO[um->mods[i]].full_name);
        strcat(str, ", ");
    }
    return str;
}       

char* mods_to_short_string(struct used_mods *um) {
    char *str = malloc(sizeof(char) * STRING_SIZE);
    strcpy(str, "");
    for (int i = 0; i < um->size; ++i)
        strcat(str, MODS_INFO[um->mods[i]].short_name);
    return str;
}
