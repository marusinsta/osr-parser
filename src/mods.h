#ifndef OSR_MODS_H
#define OSR_MODS_H

#include <inttypes.h>
#include <stdlib.h>

#define MOD_COUNT 31

struct mod {
    char *short_name, *full_name;
    uint32_t offset;
};

enum ModOffsets {
    NOFAIL           = 1 << 0,
    EASY             = 1 << 1,
    TOUCH_DEVICE     = 1 << 2,
    HIDDEN           = 1 << 3,
    HARD_ROCK        = 1 << 4,
    SUDDEN_DEATH     = 1 << 5,
    DOUBLE_TIME      = 1 << 6,
    RELAX            = 1 << 7,
    HALF_TIME        = 1 << 8,
    NIGHTCORE        = 1 << 9,
    FLASHLIGHT       = 1 << 10,
    AUTOPLAY         = 1 << 11,
    SPUN_OUT         = 1 << 12,
    AUTOPILOT        = 1 << 13,
    PERFECT          = 1 << 14,
    KEYS_4           = 1 << 15,
    KEYS_5           = 1 << 16,
    KEYS_6           = 1 << 17,
    KEYS_7           = 1 << 18,
    KEYS_8           = 1 << 19,
    FADE_IN          = 1 << 20,
    RANDOM           = 1 << 21,
    CINEMA           = 1 << 22,
    TARGET_PRACTICE  = 1 << 23,
    KEYS_9           = 1 << 24,
    COOP             = 1 << 25,
    KEYS_1           = 1 << 26,
    KEYS_3           = 1 << 27,
    KEYS_2           = 1 << 28,
    SCORE_V2         = 1 << 29,
    MIRROR           = 1 << 30
};

static const struct mod MODS_INFO[] = {
    { "NF",         "No Fail",         NOFAIL          },
    { "EZ",         "Easy",            EASY            },
    { "TD",         "Touch Device",    TOUCH_DEVICE    },
    { "HD",         "Hidden",          HIDDEN          },
    { "HR",         "HardRock",        HARD_ROCK       },
    { "SD",         "Sudden Death",    SUDDEN_DEATH    },
    { "DT",         "Double Time",     DOUBLE_TIME     },
    { "RL",         "Relax",           RELAX           },
    { "HT",         "Half Time",       HALF_TIME       },
    { "NC",         "Nightcore",       NIGHTCORE       }, // Only ever set with Double Time
    { "FL",         "Flashlight",      FLASHLIGHT      },
    { "AT",         "Autoplay",        AUTOPLAY        },
    { "SO",         "Spunout",         SPUN_OUT        },
    { "AP",         "Autopilot",       AUTOPILOT       }, // aka Relax2
    { "PF",         "Perfect",         PERFECT         }, // Only ever set with Sudden Death
    { "4K",         "4 Keys",          KEYS_4          },
    { "5K",         "5 Keys",          KEYS_5          },
    { "6K",         "6 Keys",          KEYS_6          },
    { "7K",         "7 Keys",          KEYS_7          },
    { "8K",         "8 Keys",          KEYS_8          },
    { "FI",         "Fade In",         FADE_IN         },
    { "RD",         "Random",          RANDOM          },
    { "CM",         "Cinema",          CINEMA          }, // aka LastMod
    { "TP",         "Target Practice", TARGET_PRACTICE },
    { "9K",         "9 Keys",          KEYS_9          },
    { "CP",         "Coop Keys",       COOP            },
    { "1K",         "1 Keys",          KEYS_1          },
    { "3K",         "3 Keys",          KEYS_3          },
    { "2K",         "2 Keys",          KEYS_2          },
    { "SV2",        "Score V2",        SCORE_V2        },
    { "MR",         "Mirror",          MIRROR          },
};

struct used_mods {
    size_t size;
    int8_t mods[MOD_COUNT];
};

char* mods_to_full_string(struct used_mods *um);
char* mods_to_short_string(struct used_mods *um);
struct used_mods mods_parse_from_int(int32_t mods_int);


#endif
