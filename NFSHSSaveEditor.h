#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define RAW_MAGIC "\xAC\x14"
#define SC_MAGIC "\x53\x43"
#define MC_MAGIC "\x4D\x43"
#define PSV_MAGIC "\x00\x56\x53\x50"
#define GME_MAGIC "\x31\x32\x33\x2D\x34\x35\x36\x2D\x53\x54\x44"

#define SC_HEADER_SIZE 512
#define MC_HEADER_SIZE 128
#define PSV_HEADER_SIZE 132
#define GME_HEADER_SIZE 3904

#define MC_BLOCK_COUNT 16
#define MC_BLOCK_SIZE 8192
#define MC_SIZE 131072
#define SERIAL_SIZE 10

#define RAW_SAVE_SIZE 5292

#define FRONTEND_START 8
#define FRONTEND_SIZE 1104

#define LANGUAGE_START 840

#define PLAYER_NAME_START 876
#define PLAYER_NAME_SIZE 7

#define CAR_INFO_START 1112
#define CAR_INFO_SIZE 224

#define OWNED_CAR_SLOT_SIZE 4
#define OWNED_CAR_SLOT_COUNT 32
#define OWNED_CAR_COLOR_COUNT 16
#define EMPTY_CAR_SLOT_FLAG 0xff

#define CAR_AVAILABILITY_START 1240
#define CAR_AVAILABILITY_SIZE 48
#define CAR_VISIBILITY_START 1288
#define CAR_VISIBILITY_SIZE 48

#define TRACK_INFO_START 1336
#define TRACK_INFO_SIZE 16

#define TOURNAMENT_INFO_START 1352
#define TOURNAMENT_INFO_SIZE 176

#define TROPHIES_START 1456
#define TROPHIES_SIZE 64

#define RECORD_INFO_START 1528
#define RECORD_INFO_SIZE 3740

#define FRONTEND_CRC_START 5272
#define CAR_INFO_CRC_START 5276
#define TRACK_INFO_CRC_START 5280
#define TOURNAMENT_INFO_CRC_START 5284
#define RECORD_INFO_CRC_START 5288

typedef struct {
    const char* model;
    uint8_t upgrade;
    uint8_t color;
} NFSHSOwnedCar;

typedef struct {
    size_t start;
    const char* serial;
    const char* language;
    char name[PLAYER_NAME_SIZE + 1];
    int32_t money;
    NFSHSOwnedCar ownedcars[OWNED_CAR_SLOT_COUNT];
    uint8_t unlockallcars;
    uint8_t unlockalltracks;
    uint8_t setgoldtrophies;
} NFSHSSave;

typedef struct {
    const char* format;
    const char* path;
    uint8_t* data;
    size_t size;
    NFSHSSave* saves;
    size_t savecount;
} NFSHSSaveEditor;

const char* NFSHSSaveEditor_serials[] = {"SLUS-00826", "SLES-01876", "SLES-01788", "SLES-01789", "SLES-01790"};
const char* NFSHSSaveEditor_languages[] = {"EN/JA", "DE", "FR", "ES", "IT", "SV"};
const char* NFSHSSaveEditor_models[] = {"SLK", "Z3", "VT/Skyline", "Falcon", "Camaro", "Firebird", "DB7", "XKR", "M5", "Corvette", "550", "911", "F50", "Diablo", "CLK GTR", "F1 GTR", "Race 911", "Race VT/Skyline", "Race Corvette", "Phantom", "Titan", "Titan?", "Police Caprice", "Police VT", "Police M5", "Police Corvette", "Police 911", "Police Diablo", "Jailbird", "Traffic 1", "Traffic 2", "Traffic 3", "Traffic 4", "Traffic 5", "Traffic 6", "Traffic 7", "Traffic 8", "Traffic 9", "Traffic 10", "Traffic 11", "Traffic 12", "Traffic 13", "Traffic 14", "Traffic 15", "Traffic 16", "Traffic 17", "Traffic 18", "Traffic 19", "Traffic 20", "Traffic 21"};
const uint8_t NFSHSSaveEditor_upgrades[] = {0x00, 0x01, 0x03, 0x07};

const uint8_t NFSHSSaveEditor_table1[256] = {
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
    0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
    0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
    0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
    0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
    0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
    0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40
};

const uint8_t NFSHSSaveEditor_table2[256] = {
    0x00, 0xc0, 0xc1, 0x01, 0xc3, 0x03, 0x02, 0xc2, 0xc6, 0x06, 0x07, 0xc7, 0x05, 0xc5, 0xc4, 0x04,
    0xcc, 0x0c, 0x0d, 0xcd, 0x0f, 0xcf, 0xce, 0x0e, 0x0a, 0xca, 0xcb, 0x0b, 0xc9, 0x09, 0x08, 0xc8,
    0xd8, 0x18, 0x19, 0xd9, 0x1b, 0xdb, 0xda, 0x1a, 0x1e, 0xde, 0xdf, 0x1f, 0xdd, 0x1d, 0x1c, 0xdc,
    0x14, 0xd4, 0xd5, 0x15, 0xd7, 0x17, 0x16, 0xd6, 0xd2, 0x12, 0x13, 0xd3, 0x11, 0xd1, 0xd0, 0x10,
    0xf0, 0x30, 0x31, 0xf1, 0x33, 0xf3, 0xf2, 0x32, 0x36, 0xf6, 0xf7, 0x37, 0xf5, 0x35, 0x34, 0xf4,
    0x3c, 0xfc, 0xfd, 0x3d, 0xff, 0x3f, 0x3e, 0xfe, 0xfa, 0x3a, 0x3b, 0xfb, 0x39, 0xf9, 0xf8, 0x38,
    0x28, 0xe8, 0xe9, 0x29, 0xeb, 0x2b, 0x2a, 0xea, 0xee, 0x2e, 0x2f, 0xef, 0x2d, 0xed, 0xec, 0x2c,
    0xe4, 0x24, 0x25, 0xe5, 0x27, 0xe7, 0xe6, 0x26, 0x22, 0xe2, 0xe3, 0x23, 0xe1, 0x21, 0x20, 0xe0,
    0xa0, 0x60, 0x61, 0xa1, 0x63, 0xa3, 0xa2, 0x62, 0x66, 0xa6, 0xa7, 0x67, 0xa5, 0x65, 0x64, 0xa4,
    0x6c, 0xac, 0xad, 0x6d, 0xaf, 0x6f, 0x6e, 0xae, 0xaa, 0x6a, 0x6b, 0xab, 0x69, 0xa9, 0xa8, 0x68,
    0x78, 0xb8, 0xb9, 0x79, 0xbb, 0x7b, 0x7a, 0xba, 0xbe, 0x7e, 0x7f, 0xbf, 0x7d, 0xbd, 0xbc, 0x7c,
    0xb4, 0x74, 0x75, 0xb5, 0x77, 0xb7, 0xb6, 0x76, 0x72, 0xb2, 0xb3, 0x73, 0xb1, 0x71, 0x70, 0xb0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
    0x9c, 0x5c, 0x5d, 0x9d, 0x5f, 0x9f, 0x9e, 0x5e, 0x5a, 0x9a, 0x9b, 0x5b, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4b, 0x8b, 0x8a, 0x4a, 0x4e, 0x8e, 0x8f, 0x4f, 0x8d, 0x4d, 0x4c, 0x8c,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

int NFSHSSaveEditor_findstr(const char* s, const char* list[], const size_t n) {
    for(size_t i = 0; i < n && s; i++)
        if(strcmp(s, list[i]) == 0) return (int) i;

    return -1;
}

int NFSHSSaveEditor_findbyte(const uint8_t v, const uint8_t list[], const size_t n) {
    for(size_t i = 0; i < n; i++)
        if(list[i] == v) return (int) i;

    return -1;
}

int32_t NFSHSSaveEditor_toint(const uint8_t buf[]) {
    return (int32_t) (((uint32_t) buf[0]) | ((uint32_t) buf[1] << 8) | ((uint32_t) buf[2] << 16) | ((uint32_t) buf[3] << 24));
}

void NFSHSSaveEditor_fromint(const int32_t v, uint8_t buf[]) {
    uint32_t uv = (uint32_t) v;
    buf[0] = (uint8_t) uv;
    buf[1] = (uint8_t) (uv >> 8);
    buf[2] = (uint8_t) (uv >> 16);
    buf[3] = (uint8_t) (uv >> 24);
}

void NFSHSSaveEditor_getcrc16(const uint8_t buf[], int size, uint8_t crc[]) {
    uint8_t hi = 0xfb;
    uint8_t lo = 0xea;

    while(size >= 4) {
        const uint8_t idx = buf[0] ^ lo;

        hi = buf[1] ^ NFSHSSaveEditor_table1[idx] ^ hi;

        const uint8_t b1 = buf[2] ^ NFSHSSaveEditor_table1[hi] ^ NFSHSSaveEditor_table2[idx];
        const uint8_t b2 = buf[3] ^ NFSHSSaveEditor_table1[b1] ^ NFSHSSaveEditor_table2[hi];

        hi = NFSHSSaveEditor_table2[b2];
        lo = NFSHSSaveEditor_table1[b2] ^ NFSHSSaveEditor_table2[b1];

        buf += 4;
        size -= 4;
    }

    while(size > 0) {
        const uint8_t idx = buf[0] ^ lo;

        lo = NFSHSSaveEditor_table1[idx] ^ hi;
        hi = NFSHSSaveEditor_table2[idx];

        buf++;
        size--;
    }

    crc[0] = lo;
    crc[1] = hi;
    crc[2] = 0;
    crc[3] = 0;
}

void NFSHSSaveEditor_clear(NFSHSSaveEditor* editor) {
    if(!editor) return;
    if(editor->data) free(editor->data);
    if(editor->saves) free(editor->saves);
    memset(editor, 0, sizeof(NFSHSSaveEditor));
}

void NFSHSSaveEditor_parseinfo(NFSHSSaveEditor* editor) {
    for(size_t i = 0; i < editor->savecount; i++) {
        const uint8_t lanidx = editor->data[editor->saves[i].start + LANGUAGE_START];
        editor->saves[i].language = lanidx < ARRAY_SIZE(NFSHSSaveEditor_languages) ? NFSHSSaveEditor_languages[lanidx] : NFSHSSaveEditor_languages[0];

        memcpy(editor->saves[i].name, editor->data + editor->saves[i].start + PLAYER_NAME_START, PLAYER_NAME_SIZE);
        editor->saves[i].name[PLAYER_NAME_SIZE] = '\0';

        editor->saves[i].money = NFSHSSaveEditor_toint(editor->data + editor->saves[i].start + TOURNAMENT_INFO_START);

        const size_t pos = editor->saves[i].start + CAR_INFO_START;
        for(size_t j = 0; j < OWNED_CAR_SLOT_COUNT; j++) {
            const uint8_t modidx = editor->data[pos + OWNED_CAR_SLOT_SIZE * j];
            NFSHSOwnedCar car = {NULL, 0, 0};

            if(modidx < ARRAY_SIZE(NFSHSSaveEditor_models)) {
                car.model = NFSHSSaveEditor_models[modidx];

                const int upgidx = NFSHSSaveEditor_findbyte(editor->data[pos + OWNED_CAR_SLOT_SIZE * j + 1], NFSHSSaveEditor_upgrades, ARRAY_SIZE(NFSHSSaveEditor_upgrades));
                if(upgidx > -1) car.upgrade = (uint8_t) upgidx;

                const uint8_t colidx = editor->data[pos + OWNED_CAR_SLOT_SIZE * j + 2];
                if(colidx < OWNED_CAR_COLOR_COUNT) car.color = colidx;
            }

            editor->saves[i].ownedcars[j] = car;
        }

        editor->saves[i].unlockallcars = 0;
        editor->saves[i].unlockalltracks = 0;
        editor->saves[i].setgoldtrophies = 0;
    }
}

void NFSHSSaveEditor_parse(NFSHSSaveEditor* editor) {
    if(editor->size >= RAW_SAVE_SIZE) {
        if(memcmp(editor->data, RAW_MAGIC, sizeof(RAW_MAGIC) - 1) == 0) {
            editor->saves = (NFSHSSave*) malloc(sizeof(NFSHSSave));

            if(editor->saves) {
                editor->format = "RAW";
                NFSHSSave save;
                save.start = 0;
                save.serial = NULL;
                editor->saves[0] = save;
                editor->savecount = 1;
                NFSHSSaveEditor_parseinfo(editor);
            }

            return;
        }
    }

    if(editor->size >= SC_HEADER_SIZE + RAW_SAVE_SIZE) {
        if(memcmp(editor->data, SC_MAGIC, sizeof(SC_MAGIC) - 1) == 0) {
            editor->saves = (NFSHSSave*) malloc(sizeof(NFSHSSave));

            if(editor->saves) {
                editor->format = "SC";
                NFSHSSave save;
                save.start = SC_HEADER_SIZE;
                save.serial = NULL;
                editor->saves[0] = save;
                editor->savecount = 1;
                NFSHSSaveEditor_parseinfo(editor);
            }

            return;
        }
    }

    if(editor->size >= PSV_HEADER_SIZE + SC_HEADER_SIZE + RAW_SAVE_SIZE) {
        if(memcmp(editor->data, PSV_MAGIC, sizeof(PSV_MAGIC) - 1) == 0) {
            char serial[SERIAL_SIZE + 1];
            serial[SERIAL_SIZE] = '\0';

            memcpy(serial, editor->data + 102, sizeof(serial) - 1);
            const int seridx = NFSHSSaveEditor_findstr(serial, NFSHSSaveEditor_serials, ARRAY_SIZE(NFSHSSaveEditor_serials));

            if(seridx > -1) {
                editor->saves = (NFSHSSave*) malloc(sizeof(NFSHSSave));

                if(editor->saves) {
                    editor->format = "PSV";
                    NFSHSSave save;
                    save.start = PSV_HEADER_SIZE + SC_HEADER_SIZE;
                    save.serial = NFSHSSaveEditor_serials[seridx];
                    editor->saves[0] = save;
                    editor->savecount = 1;
                    NFSHSSaveEditor_parseinfo(editor);
                }
            }

            return;
        }
    }

    size_t gmeshift = 0;
    const char* format = NULL;

    if(editor->size >= MC_SIZE && memcmp(editor->data, MC_MAGIC, sizeof(MC_MAGIC) - 1) == 0) {
       gmeshift = 0;
       format = "MC";
    }
    else if(editor->size >= GME_HEADER_SIZE + MC_SIZE && memcmp(editor->data, GME_MAGIC, sizeof(GME_MAGIC) - 1) == 0) {
       gmeshift = GME_HEADER_SIZE;
       format = "GME";
    }

    if(format) {
        long starts[MC_BLOCK_COUNT];
        int serials[MC_BLOCK_COUNT];

        size_t n = 0;
        char serial[SERIAL_SIZE + 1];
        serial[SERIAL_SIZE] = '\0';

        for(size_t i = 1; i < MC_BLOCK_COUNT; i++) {
            starts[i] = -1;
            serials[i] = -1;

            memcpy(serial, editor->data + gmeshift + MC_HEADER_SIZE * i + 12, sizeof(serial) - 1);
            const int seridx = NFSHSSaveEditor_findstr(serial, NFSHSSaveEditor_serials, ARRAY_SIZE(NFSHSSaveEditor_serials));

            if(editor->data[gmeshift + MC_HEADER_SIZE * i] == 0x51 && seridx > -1) {
                starts[i] = gmeshift + MC_BLOCK_SIZE * i + SC_HEADER_SIZE;
                serials[i] = seridx;
                n++;
            }
        }

        if(n > 0) {
            editor->saves = (NFSHSSave*) malloc(n * sizeof(NFSHSSave));

            if(editor->saves) {
                editor->format = format;

                size_t j = 0;
                NFSHSSave save;

                for(size_t i = 1; i < MC_BLOCK_COUNT; i++) {
                    if(starts[i] > -1 && serials[i] > -1) {
                        save.start = (size_t) starts[i];
                        save.serial = NFSHSSaveEditor_serials[serials[i]];
                        editor->saves[j++] = save;
                    }
                }

                editor->savecount = n;
                NFSHSSaveEditor_parseinfo(editor);
            }
        }
    }
}

void NFSHSSaveEditor_init(NFSHSSaveEditor* editor, const char* path) {
    if(!editor || !path) return;

    FILE* fp = fopen(path, "rb");
    if(!fp) return;

    fseek(fp, 0, SEEK_END);

    const long fs = ftell(fp);
    if(fs <= 0) {
        fclose(fp);
        return;
    }

    uint8_t* data = (uint8_t*) malloc(((size_t) fs) * sizeof(uint8_t));
    if(!data) {
        fclose(fp);
        return;
    }

    fseek(fp, 0, SEEK_SET);

    if(fread(data, sizeof(uint8_t), (size_t) fs, fp) != (size_t) fs) {
        fclose(fp);
        free(data);
        return;
    }

    fclose(fp);

    NFSHSSaveEditor_clear(editor);
    editor->path = path;
    editor->data = data;
    editor->size = (size_t) fs;
    NFSHSSaveEditor_parse(editor);
}

void NFSHSSaveEditor_fixcrc16(NFSHSSaveEditor* editor) {
    uint8_t crc[sizeof(uint32_t)];

    for(size_t i = 0; i < editor->savecount; i++) {
        NFSHSSaveEditor_getcrc16(editor->data + editor->saves[i].start + FRONTEND_START, FRONTEND_SIZE, crc);
        memcpy(editor->data + editor->saves[i].start + FRONTEND_CRC_START, crc, sizeof(crc));

        NFSHSSaveEditor_getcrc16(editor->data + editor->saves[i].start + CAR_INFO_START, CAR_INFO_SIZE, crc);
        memcpy(editor->data + editor->saves[i].start + CAR_INFO_CRC_START, crc, sizeof(crc));

        NFSHSSaveEditor_getcrc16(editor->data + editor->saves[i].start + TRACK_INFO_START, TRACK_INFO_SIZE, crc);
        memcpy(editor->data + editor->saves[i].start + TRACK_INFO_CRC_START, crc, sizeof(crc));

        NFSHSSaveEditor_getcrc16(editor->data + editor->saves[i].start + TOURNAMENT_INFO_START, TOURNAMENT_INFO_SIZE, crc);
        memcpy(editor->data + editor->saves[i].start + TOURNAMENT_INFO_CRC_START, crc, sizeof(crc));

        NFSHSSaveEditor_getcrc16(editor->data + editor->saves[i].start + RECORD_INFO_START, RECORD_INFO_SIZE, crc);
        memcpy(editor->data + editor->saves[i].start + RECORD_INFO_CRC_START, crc, sizeof(crc));
    }
}

void NFSHSSaveEditor_write(NFSHSSaveEditor* editor) {
    FILE* fp = fopen(editor->path, "wb");
    if(fp) {
        fwrite(editor->data, sizeof(uint8_t), editor->size, fp);
        fclose(fp);
    }
}

void NFSHSSaveEditor_updatelang(NFSHSSaveEditor* editor, const size_t saveidx, const char* lan) {
    if(saveidx >= editor->savecount) return;
    const int lanidx = NFSHSSaveEditor_findstr(lan, NFSHSSaveEditor_languages, ARRAY_SIZE(NFSHSSaveEditor_languages));
    editor->saves[saveidx].language = lanidx > -1 ? NFSHSSaveEditor_languages[lanidx] : NFSHSSaveEditor_languages[0];
}

void NFSHSSaveEditor_updatemoney(NFSHSSaveEditor* editor, const size_t saveidx, const int32_t money) {
    if(saveidx >= editor->savecount) return;
    editor->saves[saveidx].money = money;
}

void NFSHSSaveEditor_updateownedcar(NFSHSSaveEditor* editor, const size_t saveidx, const size_t carslotidx, const char* model, const uint8_t upgrade, const uint8_t color) {
    if(saveidx >= editor->savecount || carslotidx >= OWNED_CAR_SLOT_COUNT) return;

    const int modidx = NFSHSSaveEditor_findstr(model, NFSHSSaveEditor_models, ARRAY_SIZE(NFSHSSaveEditor_models));
    const uint8_t upgidx = upgrade < ARRAY_SIZE(NFSHSSaveEditor_upgrades) ? upgrade : 0;
    const uint8_t colidx = color < OWNED_CAR_COLOR_COUNT ? color : 0;

    editor->saves[saveidx].ownedcars[carslotidx].model = modidx > -1 ? NFSHSSaveEditor_models[modidx] : NULL;
    editor->saves[saveidx].ownedcars[carslotidx].upgrade = modidx > -1 ? upgidx : 0;
    editor->saves[saveidx].ownedcars[carslotidx].color = modidx > -1 ? colidx : 0;
}

void NFSHSSaveEditor_unlockcars(NFSHSSaveEditor* editor, const size_t saveidx) {
    if(saveidx >= editor->savecount) return;
    editor->saves[saveidx].unlockallcars = 1;
}

void NFSHSSaveEditor_unlocktracks(NFSHSSaveEditor* editor, const size_t saveidx) {
    if(saveidx >= editor->savecount) return;
    editor->saves[saveidx].unlockalltracks = 1;
}

void NFSHSSaveEditor_setgoldtrophies(NFSHSSaveEditor* editor, const size_t saveidx) {
    if(saveidx >= editor->savecount) return;
    editor->saves[saveidx].setgoldtrophies = 1;
}

void NFSHSSaveEditor_update(NFSHSSaveEditor* editor) {
    if(!editor || !editor->data || !editor->path || editor->savecount == 0) return;
    uint8_t buf[sizeof(uint32_t)];

    for(size_t i = 0; i < editor->savecount; i++) {
        const int lanidx = NFSHSSaveEditor_findstr(editor->saves[i].language, NFSHSSaveEditor_languages, ARRAY_SIZE(NFSHSSaveEditor_languages));
        editor->data[editor->saves[i].start + LANGUAGE_START] = lanidx > -1 ? (uint8_t) lanidx : 0;

        NFSHSSaveEditor_fromint(editor->saves[i].money, buf);
        memcpy(editor->data + editor->saves[i].start + TOURNAMENT_INFO_START, buf, sizeof(buf));

        for(size_t j = 0; j < OWNED_CAR_SLOT_COUNT; j++) {
            const int modidx = NFSHSSaveEditor_findstr(editor->saves[i].ownedcars[j].model, NFSHSSaveEditor_models, ARRAY_SIZE(NFSHSSaveEditor_models));
            const uint8_t upgidx = editor->saves[i].ownedcars[j].upgrade < ARRAY_SIZE(NFSHSSaveEditor_upgrades) ? editor->saves[i].ownedcars[j].upgrade : 0;
            const uint8_t colidx = editor->saves[i].ownedcars[j].color < OWNED_CAR_COLOR_COUNT ? editor->saves[i].ownedcars[j].color : 0;

            editor->data[editor->saves[i].start + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * j] = modidx > -1 ? (uint8_t) modidx : EMPTY_CAR_SLOT_FLAG;
            editor->data[editor->saves[i].start + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * j + 1] = modidx > -1 ? NFSHSSaveEditor_upgrades[upgidx] : NFSHSSaveEditor_upgrades[0];
            editor->data[editor->saves[i].start + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * j + 2] = modidx > -1 ? colidx : 0;
        }

        if(editor->saves[i].unlockallcars) {
            memset(editor->data + editor->saves[i].start + CAR_AVAILABILITY_START, 0x01, CAR_AVAILABILITY_SIZE);
            memset(editor->data + editor->saves[i].start + CAR_VISIBILITY_START, 0x01, CAR_VISIBILITY_SIZE);
        }

        if(editor->saves[i].unlockalltracks)
            memset(editor->data + editor->saves[i].start + TRACK_INFO_START, 0x01, TRACK_INFO_SIZE);

        if(editor->saves[i].setgoldtrophies)
            memset(editor->data + editor->saves[i].start + TROPHIES_START, 0x01, TROPHIES_SIZE);
    }

    NFSHSSaveEditor_fixcrc16(editor);
    NFSHSSaveEditor_write(editor);
}