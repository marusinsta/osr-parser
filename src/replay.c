#include <stdio.h>
#include "replay.h"
#include "mods.h"

void replay_print(struct replay *replay) {
    struct used_mods um = mods_parse_from_int(replay->mods);
    printf("Mode: ");
    switch (replay->mode) {
        case STD:   printf("osu!\n");           break;
        case TAIKO: printf("Taiko\n");          break;
        case CTB:   printf("Catch The Beat\n"); break;
        case MANIA: printf("Mania\n");          break;
    }
    printf("Game verison: %" PRIu32 "\n"
            "Beatmap MD5 hash: %s\n"
            "Player name: %s\n"
            "Replay MD5 hash: %s\n"
            "Score:\n"
            "\t300s: %" PRIu16 "\n"
            "\t100s: %" PRIu16 "\n"
            "\t50s: %" PRIu16 "\n"
            "\tGekis: %" PRIu16 "\n"
            "\tKatus: %" PRIu16 "\n"
            "\tMisses: %" PRIu16 "\n"
            "\tTotal score: %" PRIu32 "\n"
            "\tMax combo: %" PRIu16 "\n"
            "\tPerfect combo: %" PRIu8 "\n"
            "\tMods used: %s\n"
            "Time stamp: %" PRIu64 "\n"
            "Online score ID: %" PRIu64 "\n",
            replay->version, replay-> beatmap_hash, replay->player_name, 
            replay->replay_hash, replay->acc_300, replay->acc_100, replay->acc_50,
            replay->acc_geki, replay->acc_katu, replay->acc_miss,
            replay->score, replay->max_combo, replay->perfect,
            mods_to_full_string(&um), replay->timestamp, replay->score_id
    );
    lifebar_print(&replay->lifebar);
    /* replay_data_print(&replay->replay_data); */
}

void replay_free(struct replay *replay) {
    if (replay != NULL) {
        free(replay->beatmap_hash);
        free(replay->player_name);
        free(replay->replay_hash);
        lifebar_free(&replay->lifebar);
        replay_data_free(&replay->replay_data);
    }
}
