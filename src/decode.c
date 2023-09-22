#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <lzma.h>
#include <stdbool.h>

#include "decode.h"
#include "replay.h"
#include "mods.h"
#include "lifebar.h"
#include "replay_data.h"

static uint8_t read_byte(FILE *osr) { return fgetc(osr); }

static uint16_t read_short(FILE *osr) { return fgetc(osr) + (fgetc(osr) << 8); }

static uint32_t read_int(FILE *osr) {
    uint32_t ret = 0;
    uint32_t pow = 1;
    for (size_t i = 0; i < 4; ++i) {
        ret += fgetc(osr) * pow;
        pow <<= 8;
    }
    return ret;
}

static uint64_t read_long(FILE *osr) {
    uint64_t ret = 0;
    uint64_t pow = 1;
    for (size_t i = 0; i < 8; ++i) {
        ret += fgetc(osr) * pow;
        pow <<= 8;
    }
    return ret;
}

static uint64_t read_ULEB128(FILE *osr) {
    uint64_t res = 0;
    uint8_t shift = 0;
    while (true) {
        uint8_t byte = fgetc(osr);
        res = res | (byte & 0x7F) << shift; // 0x7F = 01111111
        if ((byte & 0x80) == 0x00)          // 0x80 = 10000000
            break;
        shift += 7;
    }
    return res;
}

static char* read_osu_string(FILE *osr) {
    uint8_t flag = fgetc(osr);
    char *str;
    if (flag == 0x0B) {
        uint64_t size = read_ULEB128(osr);
        str = malloc(sizeof(char) * (size + 1));
        fgets(str, size + 1, osr);
        return str;
    }
    return "";
}

static char* read_replay_data(FILE *osr, uint32_t replay_length) {
    /* Based on kobolabs/liblzma/blob/master/doc/examples/02_decompress.c */

    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_action action = LZMA_RUN;
    lzma_ret ret = lzma_alone_decoder(&strm, UINT64_MAX);
    if (ret == LZMA_OK) {
        size_t decoded_data_length = replay_length;
        char *decoded_data = malloc(decoded_data_length);
        strcpy(decoded_data, "");
        uint8_t inbuf[16384];
        uint8_t outbuf[16384];
        while (true) {
            if (strm.avail_in == 0 && !feof(osr) && strm.total_in < replay_length) {
                strm.next_in = inbuf;
                if (strm.total_in + sizeof(inbuf) < replay_length)
                    strm.avail_in = fread(inbuf, 1, sizeof(inbuf), osr);
                else
                    strm.avail_in = fread(inbuf, 1, replay_length - strm.total_in, osr);
                if (ferror(osr)) {
                    fprintf(stderr, "Read error\n");
                    return NULL;
                }
            }
            lzma_ret ret = lzma_code(&strm, action);
            size_t write_size = sizeof(outbuf) - strm.avail_out;
            if (strm.avail_out == 0 || ret == LZMA_STREAM_END) {
                if (write_size + strm.total_out > decoded_data_length) {
                    decoded_data_length *= 2;
                    decoded_data = realloc(decoded_data, decoded_data_length);
                }
                strncat(decoded_data, (char*) outbuf, write_size);

                strm.next_out = outbuf;
                strm.avail_out = sizeof(outbuf);
            }
            if (feof(osr) || (strm.total_in == replay_length && strm.avail_in == 0))
                action = LZMA_FINISH;
            if (action == LZMA_FINISH) {
                lzma_end(&strm);
                return decoded_data;
            }
            if (ret != LZMA_OK) {
                if (ret == LZMA_STREAM_END)
                    return NULL;
                fprintf(stderr, "Couldn't decode replay data");
                return NULL;
            }
        }
    }
    return NULL;
}

static int64_t windows_ticks_to_unix(int64_t timestamp) {
    const uint64_t WINDOWS_TICK = 10000000; // Ticks per 1 second
    const uint64_t SEC_TO_UNIX_EPOCH = 62135596800; // Time between ticks and epoch start
    return timestamp / WINDOWS_TICK - SEC_TO_UNIX_EPOCH;
}

bool check_if_osr(char *path) {
    size_t len = strlen(path);
    return strcmp((path + len - 4), ".osr") == 0;
}

struct replay replay_parse(char *path) {
    FILE *osr = fopen(path, "rb");
    if (osr == NULL) {
        printf("No such file\n");
        struct replay rep = {};
        return rep;
    }
    else {
        enum     GameMode mode             = read_byte(osr);
        uint32_t ver                       = read_int(osr);
        char     *beatmap_hash             = read_osu_string(osr);
        char     *username                 = read_osu_string(osr);
        char     *replay_hash              = read_osu_string(osr);
        uint16_t acc_300                   = read_short(osr);
        uint16_t acc_100                   = read_short(osr);
        uint16_t acc_50                    = read_short(osr);
        uint16_t acc_geki                  = read_short(osr);
        uint16_t acc_katu                  = read_short(osr);
        uint16_t acc_miss                  = read_short(osr);
        uint32_t total_score               = read_int(osr);
        uint16_t max_combo                 = read_short(osr);
        uint8_t  perfect                   = read_byte(osr);
        uint32_t mods                      = read_int(osr);
        char     *lifebar_str              = read_osu_string(osr);
        uint64_t timestamp                 = read_long(osr);
        uint32_t replay_length             = read_int(osr);
        char     *uncompressed_replay_data = read_replay_data(osr, replay_length);
        uint64_t score_id                  = read_long(osr);
        fclose(osr);
        struct replay rep = {
                mode, ver, beatmap_hash, username, replay_hash,
                acc_300, acc_100, acc_50, acc_geki, acc_katu, acc_miss,
                total_score, max_combo, perfect, mods,
                lifebar_parse(lifebar_str), timestamp,
                replay_data_parse(uncompressed_replay_data), score_id
        };
        return rep;
    }
}
