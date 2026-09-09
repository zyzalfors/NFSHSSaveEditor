#ifndef NFSHSSAVEEDITOR_H
#define NFSHSSAVEEDITOR_H

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

#define CAR_AVAILABILITY_START 1240
#define CAR_AVAILABILITY_SIZE 45
#define CAR_VISIBILITY_START 1288
#define CAR_VISIBILITY_SIZE 45

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

typedef enum {UNK, RAW, SC, MC, PSV, GME} FORMAT;
typedef enum {LANGUAGE, CAR, CARS, TRACKS, MONEY, TROPHIES} TYPE;

typedef struct {
    const char* path;
    uint8_t* data;
    size_t size;
    size_t* saveoffsets;
    size_t savecount;
    FORMAT format;
} NFSHSSaveEditor;

static const char* serials[] = {"SLUS-00826", "SLES-01788", "SLES-01789", "SLES-01790", "SLPS-02099"};
static const char* languages[] = {"en", "ge", "fr", "sp", "it", "sw"};
static const char* cars[] = {"slk", "z3", "hsvvt/skyline", "falcon", "camaro", "firebird", "db7", "xkr", "m5", "corvette", "550", "911", "f50", "diablo", "clk", "f1", "race_911", "race_hsvvt/race_skyline", "race_corvette", "phantom", "titan", NULL, "cop_caprice", "cop_hsvvt", "cop_m5", "cop_corvette", "cop_911", "cop_diablo", "jailbird"};
static const uint8_t upgradedata[] = {0x00, 0x01, 0x03, 0x07};
static const uint8_t unlockedcardata = 0x01;
static const uint8_t alltracksdata[] = {0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t goldtrophydata = 0x01;

static int find(const char* str, const char* list[], const size_t n) {
    for(size_t i = 0; i < n; i++)
        if(list[i] && strcmp(str, list[i]) == 0) return i;

    return -1;
}

static void lower(char* s) {
    for(size_t i = 0; s[i] != '\0'; i++)
        s[i] = (char) tolower((unsigned char) s[i]);
}

static int32_t toint(const uint8_t* buf) {
    return (int32_t) (((uint32_t) buf[0]) | ((uint32_t) buf[1] << 8) | ((uint32_t) buf[2] << 16) | ((uint32_t) buf[3] << 24));
}

static void fromint(const int32_t val, uint8_t* buf) {
    uint32_t v = (uint32_t) val;
    buf[0] = (uint8_t) v;
    buf[1] = (uint8_t) (v >> 8);
    buf[2] = (uint8_t) (v >> 16);
    buf[3] = (uint8_t) (v >> 24);
}

static const uint8_t table1[256] = {
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

static const uint8_t table2[256] = {
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

static void getcrc16(const uint8_t* buf, int size, uint8_t crc[]) {
    uint8_t hi = 0xfb;
    uint8_t lo = 0xea;

    while(size >= 4) {
        uint8_t idx = buf[0] ^ lo;

        hi = buf[1] ^ table1[idx] ^ hi;

        uint8_t b1 = buf[2] ^ table1[hi] ^ table2[idx];
        uint8_t b2 = buf[3] ^ table1[b1] ^ table2[hi];

        hi = table2[b2];
        lo = table1[b2] ^ table2[b1];

        buf += 4;
        size -= 4;
    }

    while(size > 0) {
        uint8_t idx = buf[0] ^ lo;

        lo = table1[idx] ^ hi;
        hi = table2[idx];

        buf++;
        size--;
    }

    crc[0] = lo;
    crc[1] = hi;
    crc[2] = 0;
    crc[3] = 0;
}

void clear(NFSHSSaveEditor* editor) {
    free(editor->data);
    free(editor->saveoffsets);
    editor->path = NULL;
    editor->data = NULL;
    editor->size = 0;
    editor->saveoffsets = NULL;
    editor->savecount = 0;
    editor->format = UNK;
}

static void parse(NFSHSSaveEditor* editor) {
    if(editor->size >= RAW_SAVE_SIZE) {
        if(memcmp(editor->data, RAW_MAGIC, sizeof(RAW_MAGIC) - 1) == 0) {
            editor->saveoffsets = (size_t*) malloc(sizeof(size_t));
            editor->format = RAW;

            if(editor->saveoffsets) {
                editor->saveoffsets[0] = 0;
                editor->savecount = 1;
            }

            return;
        }
    }

    if(editor->size >= SC_HEADER_SIZE + RAW_SAVE_SIZE) {
        if(memcmp(editor->data, SC_MAGIC, sizeof(SC_MAGIC) - 1) == 0) {
            editor->saveoffsets = (size_t*) malloc(sizeof(size_t));
            editor->format = SC;

            if(editor->saveoffsets) {
                editor->saveoffsets[0] = SC_HEADER_SIZE;
                editor->savecount = 1;
            }

            return;
        }
    }

    if(editor->size >= PSV_HEADER_SIZE + SC_HEADER_SIZE + RAW_SAVE_SIZE) {
        if(memcmp(editor->data, PSV_MAGIC, sizeof(PSV_MAGIC) - 1) == 0) {
            char serial[SERIAL_SIZE + 1];
            serial[SERIAL_SIZE] = '\0';
            memcpy(serial, editor->data + 102, sizeof(serial) - 1);

            if(find(serial, serials, ARRAY_SIZE(serials)) > -1) {
                editor->saveoffsets = (size_t*) malloc(sizeof(size_t));
                editor->format = PSV;

                if(editor->saveoffsets) {
                    editor->saveoffsets[0] = PSV_HEADER_SIZE + SC_HEADER_SIZE;
                    editor->savecount = 1;
                }

                return;
            }
        }
    }

    long gmeshift = -1;
    if(editor->size >= MC_SIZE && memcmp(editor->data, MC_MAGIC, sizeof(MC_MAGIC) - 1) == 0) {
       editor->format = MC;
       gmeshift = 0;
    }
    if(editor->size >= GME_HEADER_SIZE + MC_SIZE && memcmp(editor->data, GME_MAGIC, sizeof(GME_MAGIC) - 1) == 0) {
       editor->format = GME;
       gmeshift = GME_HEADER_SIZE;
    }

    if(gmeshift != -1) {
        long temp[MC_BLOCK_COUNT];
        size_t n = 0;

        char serial[SERIAL_SIZE + 1];
        serial[SERIAL_SIZE] = '\0';

        for(size_t i = 1; i < MC_BLOCK_COUNT; i++) {
            temp[i] = -1;

            uint8_t flag = editor->data[gmeshift + MC_HEADER_SIZE * i];
            memcpy(serial, editor->data + gmeshift + MC_HEADER_SIZE * i + 12, sizeof(serial) - 1);

            if(flag == 0x51 && find(serial, serials, ARRAY_SIZE(serials)) > -1) {
                temp[i] = gmeshift + MC_BLOCK_SIZE * i + SC_HEADER_SIZE;
                n++;
            }
        }

        if(n > 0) {
            editor->saveoffsets = (size_t*) malloc(n * sizeof(size_t));

            if(editor->saveoffsets) {
                editor->savecount = n;
                size_t k = 0;

                for(size_t i = 1; i < MC_BLOCK_COUNT; i++)
                    if(temp[i] != -1) editor->saveoffsets[k++] = (size_t) temp[i];
            }
        }
    }
}

NFSHSSaveEditor init(const char* path) {
    NFSHSSaveEditor editor;
    editor.path = path;
    editor.data = NULL;
    editor.size = 0;
    editor.saveoffsets = NULL;
    editor.savecount = 0;
    editor.format = UNK;
    if(!path) return editor;

    FILE* fp = fopen(editor.path, "rb");
    if(!fp) return editor;

    fseek(fp, 0, SEEK_END);

    long fs = ftell(fp);
    if(fs <= 0) {
        fclose(fp);
        return editor;
    }

    editor.data = (uint8_t*) malloc(fs * sizeof(uint8_t));
    if(!editor.data) {
        fclose(fp);
        return editor;
    }

    fseek(fp, 0, SEEK_SET);

    if(fread(editor.data, sizeof(uint8_t), (size_t) fs, fp) != (size_t) fs) {
        fclose(fp);
        free(editor.data);
        editor.data = NULL;
        return editor;
    }

    editor.size = (size_t) fs;
    fclose(fp);

    parse(&editor);
    return editor;
}

void print(NFSHSSaveEditor* editor) {
    switch(editor->format) {
        case UNK:
            printf("Format: unknown\n");
            break;

        case RAW:
            printf("Format: RAW\n");
            break;

        case SC:
            printf("Format: SC\n");
            break;

        case MC:
            printf("Format: MC\n");
            break;

        case PSV:
            printf("Format: PSV\n");
            break;

        case GME:
            printf("Format: GME\n");
            break;
    }

    for(size_t k = 0; k < editor->savecount; k++) {
        long pos = editor->saveoffsets[k] + LANGUAGE_START;
        int langidx = editor->data[pos] < ARRAY_SIZE(languages) ? editor->data[pos] : -1;

       if(langidx > -1)
           printf("Language at %ld: %s\n", pos, languages[langidx]);
       else
           printf("Language at %ld: ?\n", pos);

        pos = editor->saveoffsets[k] + PLAYER_NAME_START;
        char name[PLAYER_NAME_SIZE + 1];
        name[PLAYER_NAME_SIZE] = '\0';
        memcpy(name, editor->data + pos, sizeof(name) - 1);
        printf("Player name at %ld: %s\n", pos, name);

        pos = editor->saveoffsets[k] + CAR_INFO_START;
        printf("Owned cars at %ld:\n", pos);

        for(size_t i = 0; i < OWNED_CAR_SLOT_COUNT; i++) {
            if(editor->data[pos + OWNED_CAR_SLOT_SIZE * i] == 0xFF) break;

            int caridx = editor->data[pos + OWNED_CAR_SLOT_SIZE * i] < ARRAY_SIZE(cars) ? editor->data[pos + OWNED_CAR_SLOT_SIZE * i] : -1;
            int upgidx = -1;
            int color = editor->data[pos + OWNED_CAR_SLOT_SIZE * i + 2];

            if(caridx > -1 && cars[caridx])
                printf("slot %d | car %s | ", (int) i, cars[caridx]);
            else
                printf("slot %d | car ? | ", (int) i);

            for(size_t j = 0; j < ARRAY_SIZE(upgradedata); j++)
                if(editor->data[pos + OWNED_CAR_SLOT_SIZE * i + 1] == upgradedata[j]) upgidx = (int) j;

            if(upgidx > -1) printf("upgrade %d | ", upgidx);
            else printf("upgrade ? | ");

            printf("color %d\n", color);
        }

        pos = editor->saveoffsets[k] + TOURNAMENT_INFO_START;
        printf("Money at %ld: %d\n", pos, toint(editor->data + pos));

        uint8_t crc[sizeof(uint32_t)];

        pos = editor->saveoffsets[k] + FRONTEND_CRC_START;
        getcrc16(editor->data + editor->saveoffsets[k] + FRONTEND_START, FRONTEND_SIZE, crc);

        if(memcmp(editor->data + pos, crc, sizeof(crc)) == 0)
            printf("Frontend CRC at %ld: valid\n", pos);
        else
            printf("Frontend CRC at %ld: invalid\n", pos);

        pos = editor->saveoffsets[k] + CAR_INFO_CRC_START;
        getcrc16(editor->data + editor->saveoffsets[k] + CAR_INFO_START, CAR_INFO_SIZE, crc);

        if(memcmp(editor->data + pos, crc, sizeof(crc)) == 0)
            printf("Car info CRC at %ld: valid\n", pos);
        else
            printf("Car info CRC at %ld: invalid\n", pos);

        pos = editor->saveoffsets[k] + TRACK_INFO_CRC_START;
        getcrc16(editor->data + editor->saveoffsets[k] + TRACK_INFO_START, TRACK_INFO_SIZE, crc);

        if(memcmp(editor->data + pos, crc, sizeof(crc)) == 0)
            printf("Track info CRC at %ld: valid\n", pos);
        else
            printf("Track info CRC at %ld: invalid\n", pos);

        pos = editor->saveoffsets[k] + TOURNAMENT_INFO_CRC_START;
        getcrc16(editor->data + editor->saveoffsets[k] + TOURNAMENT_INFO_START, TOURNAMENT_INFO_SIZE, crc);

        if(memcmp(editor->data + pos, crc, sizeof(crc)) == 0)
            printf("Tournament info CRC at %ld: valid\n", pos);
        else
            printf("Tournament info CRC at %ld: invalid\n", pos);

        pos = editor->saveoffsets[k] + RECORD_INFO_CRC_START;
        getcrc16(editor->data + editor->saveoffsets[k] + RECORD_INFO_START, RECORD_INFO_SIZE, crc);

        if(memcmp(editor->data + pos, crc, sizeof(crc)) == 0)
            printf("Record info CRC at %ld: valid\n", pos);
        else
            printf("Record info CRC at %ld: invalid\n", pos);

        printf("\n");
    }
}

void fix(NFSHSSaveEditor* editor) {
    uint8_t crc[sizeof(uint32_t)];
    for(size_t k = 0; k < editor->savecount; k++) {
        getcrc16(editor->data + editor->saveoffsets[k] + FRONTEND_START, FRONTEND_SIZE, crc);
        memcpy(editor->data + editor->saveoffsets[k] + FRONTEND_CRC_START, crc, sizeof(crc));

        getcrc16(editor->data + editor->saveoffsets[k] + CAR_INFO_START, CAR_INFO_SIZE, crc);
        memcpy(editor->data + editor->saveoffsets[k] + CAR_INFO_CRC_START, crc, sizeof(crc));

        getcrc16(editor->data + editor->saveoffsets[k] + TRACK_INFO_START, TRACK_INFO_SIZE, crc);
        memcpy(editor->data + editor->saveoffsets[k] + TRACK_INFO_CRC_START, crc, sizeof(crc));

        getcrc16(editor->data + editor->saveoffsets[k] + TOURNAMENT_INFO_START, TOURNAMENT_INFO_SIZE, crc);
        memcpy(editor->data + editor->saveoffsets[k] + TOURNAMENT_INFO_CRC_START, crc, sizeof(crc));

        getcrc16(editor->data + editor->saveoffsets[k] + RECORD_INFO_START, RECORD_INFO_SIZE, crc);
        memcpy(editor->data + editor->saveoffsets[k] + RECORD_INFO_CRC_START, crc, sizeof(crc));
    }
}

void update(NFSHSSaveEditor* editor, const TYPE type, const int32_t* val, char* str) {
    switch(type) {
        case LANGUAGE: {
            if(!str) return;
            lower(str);

            int langidx = find(str, languages, ARRAY_SIZE(languages));
            if(langidx > -1) {
                for(size_t k = 0; k < editor->savecount; k++)
                    editor->data[editor->saveoffsets[k] + LANGUAGE_START] = (uint8_t) langidx;
            }
            else return;

            break;
        }

        case CAR: {
            if(!val || !str) return;
            lower(str);

            int caridx = find(str, cars, ARRAY_SIZE(cars));
            int upgidx = val[0] >= 0 && val[0] < (int) ARRAY_SIZE(upgradedata) ? val[0] : -1;

            if(caridx > -1 && upgidx > -1) {
                for(size_t k = 0; k < editor->savecount; k++) {
                    int slotidx = -1;

                    for(size_t i = 0; i < OWNED_CAR_SLOT_COUNT; i++) {
                        if(editor->data[editor->saveoffsets[k] + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * i] == 0xFF) {
                            slotidx = (int) i;
                            break;
                        }
                    }
                    if(slotidx == -1) slotidx = 0;

                    editor->data[editor->saveoffsets[k] + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * slotidx] = (uint8_t) caridx;
                    editor->data[editor->saveoffsets[k] + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * slotidx + 1] = upgradedata[upgidx];
                    editor->data[editor->saveoffsets[k] + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * slotidx + 2] = (uint8_t) val[1];
                }
            }
            else return;

            break;
        }

        case CARS: {
            for(size_t k = 0; k < editor->savecount; k++) {
                memset(editor->data + editor->saveoffsets[k] + CAR_AVAILABILITY_START, unlockedcardata, CAR_AVAILABILITY_SIZE);
                memset(editor->data + editor->saveoffsets[k] + CAR_VISIBILITY_START, unlockedcardata, CAR_VISIBILITY_SIZE);
            }

            break;
        }

        case TRACKS: {
            for(size_t k = 0; k < editor->savecount; k++)
                memcpy(editor->data + editor->saveoffsets[k] + TRACK_INFO_START, alltracksdata, ARRAY_SIZE(alltracksdata));

            break;
        }

        case MONEY: {
            if(!val) return;

            uint8_t buf[sizeof(uint32_t)];
            fromint(val[0], buf);

            for(size_t k = 0; k < editor->savecount; k++)
                memcpy(editor->data + editor->saveoffsets[k] + TOURNAMENT_INFO_START, buf, sizeof(buf));

            break;
        }

        case TROPHIES: {
            for(size_t k = 0; k < editor->savecount; k++)
                memset(editor->data + editor->saveoffsets[k] + TROPHIES_START, goldtrophydata, TROPHIES_SIZE);

            break;
        }
    }

    fix(editor);
}

void save(NFSHSSaveEditor* editor) {
    FILE* fp = fopen(editor->path, "wb");
    if(fp) {
        fwrite(editor->data, sizeof(uint8_t), editor->size, fp);
        fclose(fp);
    }
}

#endif