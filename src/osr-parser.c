#include <stdio.h>

#include "replay.h"
#include "decode.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong amount of parameters\n");
        return 1;
    }
    else {
        if (check_if_osr(argv[1])) {
            struct replay replay = replay_parse(argv[1]);
            replay_print(&replay);
            replay_free(&replay);
        }
        else {
            printf("Wrong file format\n");
        }
        return 0;
    }
}
