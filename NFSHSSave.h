#ifndef NFSHSSAVE_H
#define NFSHSSAVE_H

#define SIZE 5292

#define FRONTEND_START 8
#define FRONTEND_SIZE 1104

#define LANGUAGE_START 840
#define LANGUAGES_SIZE 6

#define PLAYER_NAME_START 876
#define PLAYER_NAME_SIZE 8

#define CAR_INFO_START 1112
#define CAR_INFO_SIZE 224

#define OWNED_CAR_SLOT_SIZE 4
#define OWNED_CAR_SLOT_COUNT 32
#define CAR_COUNT 29
#define UPGRADE_COUNT 4

#define CAR_AVAILABILITY_START 1240
#define CAR_AVAILABILITY_SIZE 45
#define CAR_VISIBILITY_START 1288
#define CAR_VISIBILITY_SIZE 45

#define TRACK_INFO_START 1336
#define TRACK_INFO_SIZE 16

#define TOURNAMENT_INFO_START 1352
#define TOURNAMENT_INFO_SIZE 176

#define MONEY_SIZE 4

#define TROPHIES_START 1456
#define TROPHIES_SIZE 64

#define RECORD_INFO_START 1528
#define RECORD_INFO_SIZE 3740

#define FRONTEND_CRC_START 5272
#define CAR_INFO_CRC_START 5276
#define TRACK_INFO_CRC_START 5280
#define TOURNAMENT_INFO_CRC_START 5284
#define RECORD_INFO_CRC_START 5288
#define CRC_SIZE 4

typedef enum {LANGUAGE, CAR, CARS, TRACKS, MONEY, TROPHIES} TYPE;

typedef struct {
    char* path;
    uint8_t* data;
    size_t size;
    long start;
} NFSHSSave;

static const char* languages[LANGUAGES_SIZE] = {"en", "ge", "fr", "sp", "it", "sw"};

static const char* cars[CAR_COUNT] = {"slk", "z3", "hsvvt", "falcon", "camaro", "firebird", "db7", "xkr", "m5", "corvette",
                                      "550", "911", "f50", "diablo", "clk", "f1", "race_911", NULL, "race_corvette", "phantom",
                                      "titan", NULL, "cop_caprice", "cop_hsvvt", "cop_m5", "cop_corvette", "cop_911", "cop_diablo", "jailbird"};

static const uint8_t upgradedata[UPGRADE_COUNT] = {0x00, 0x01, 0x03, 0x07};

static const uint8_t unlockedcardata = 0x01;

static const uint8_t alltracksdata[TRACK_INFO_SIZE] = {0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

static const uint8_t goldtrophydata = 0x01;

static void lower(char* s) {
    for(int i = 0; s[i] != '\0'; i++)
        s[i] = tolower((unsigned char) s[i]);
}

static int32_t toint(uint8_t* buf) {
    return (int32_t) (((uint32_t) buf[0]) | ((uint32_t) buf[1] << 8) | ((uint32_t) buf[2] << 16) | ((uint32_t) buf[3] << 24));
}

static void fromint(int32_t val, uint8_t* buf) {
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

static void getcrc16(uint8_t* buf, int size, uint8_t crc[]) {
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

void inits(NFSHSSave* save, char* path, long start) {
    if(!save) return;

    save->path = path;
    save->data = NULL;
    save->size = 0;
    save->start = start;

    FILE* fp = fopen(save->path, "rb");
    if(!fp) return;

    fseek(fp, 0, SEEK_END);

    long fs = ftell(fp);
    if(save->start < 0 || fs < 0 || fs - save->start < SIZE) {
        fclose(fp);
        return;
    }

    save->data = (uint8_t*) malloc(fs * sizeof(uint8_t));
    if(!save->data) {
        fclose(fp);
        return;
    }

    fseek(fp, 0, SEEK_SET);

    if(fread(save->data, sizeof(uint8_t), (size_t) fs, fp) != (size_t) fs) {
        fclose(fp);
        free(save->data);
        save->data = NULL;
        return;
    }

    save->size = (size_t) fs;
    fclose(fp);
}

void clears(NFSHSSave* save) {
    if(save && save->data) free(save->data);
}

void prints(NFSHSSave* save) {
    if(!save || !save->data || save->size == 0) return;

    long pos = save->start + LANGUAGE_START;
    int langidx = save->data[pos] < LANGUAGES_SIZE ? save->data[pos] : -1;

    if(langidx > -1)
       printf("Language at %ld: %s\n", pos, languages[langidx]);
    else
       printf("Language at %ld: invalid\n", pos);

    pos = save->start + PLAYER_NAME_START;
    char name[PLAYER_NAME_SIZE];
    memcpy(name, save->data + pos, PLAYER_NAME_SIZE);
    name[PLAYER_NAME_SIZE - 1] = '\0';
    printf("Player name at %ld: %s\n", pos, name);

    pos = save->start + CAR_INFO_START;
    printf("Owned cars at %ld:\n", pos);

    for(int i = 0; i < OWNED_CAR_SLOT_COUNT; i++) {
        if(save->data[pos + OWNED_CAR_SLOT_SIZE * i] == 0xFF) break;

        int caridx = save->data[pos + OWNED_CAR_SLOT_SIZE * i] < CAR_COUNT ? save->data[pos + OWNED_CAR_SLOT_SIZE * i] : -1;
        int upgidx = -1;
        int color = save->data[pos + OWNED_CAR_SLOT_SIZE * i + 2];

        if(caridx > -1 && cars[caridx]) printf("slot %d | car %s | ", i, cars[caridx]);
        else printf("slot %d | car inv. | ", i);

        for(int j = 0; j < UPGRADE_COUNT; j++)
            if(save->data[pos + OWNED_CAR_SLOT_SIZE * i + 1] == upgradedata[j]) upgidx = j;

        if(upgidx > -1) printf("upgrade %d | ", upgidx);
        else printf("upgrade inv. | ");

        printf("color %d\n", color);
    }

    pos = save->start + TOURNAMENT_INFO_START;
    printf("Money at %ld: %d\n", pos, toint(save->data + pos));

    uint8_t crc[CRC_SIZE];

    pos = save->start + FRONTEND_CRC_START;
    getcrc16(save->data + save->start + FRONTEND_START, FRONTEND_SIZE, crc);

    if(memcmp(save->data + pos, crc, CRC_SIZE) == 0)
        printf("Frontend CRC at %ld: valid\n", pos);
    else
        printf("Frontend CRC at %ld: invalid\n", pos);

    pos = save->start + CAR_INFO_CRC_START;
    getcrc16(save->data + save->start + CAR_INFO_START, CAR_INFO_SIZE, crc);

    if(memcmp(save->data + pos, crc, CRC_SIZE) == 0)
        printf("Car info CRC at %ld: valid\n", pos);
    else
        printf("Car info CRC at %ld: invalid\n", pos);

    pos = save->start + TRACK_INFO_CRC_START;
    getcrc16(save->data + save->start + TRACK_INFO_START, TRACK_INFO_SIZE, crc);

    if(memcmp(save->data + pos, crc, CRC_SIZE) == 0)
        printf("Track info CRC at %ld: valid\n", pos);
    else
        printf("Track info CRC at %ld: invalid\n", pos);

    pos = save->start + TOURNAMENT_INFO_CRC_START;
    getcrc16(save->data + save->start + TOURNAMENT_INFO_START, TOURNAMENT_INFO_SIZE, crc);

    if(memcmp(save->data + pos, crc, CRC_SIZE) == 0)
        printf("Tournament info CRC at %ld: valid\n", pos);
    else
        printf("Tournament info CRC at %ld: invalid\n", pos);

    pos = save->start + RECORD_INFO_CRC_START;
    getcrc16(save->data + save->start + RECORD_INFO_START, RECORD_INFO_SIZE, crc);

    if(memcmp(save->data + pos, crc, CRC_SIZE) == 0)
        printf("Record info CRC at %ld: valid\n", pos);
    else
        printf("Record info CRC at %ld: invalid\n", pos);
}

void fixs(NFSHSSave* save) {
    if(!save || !save->data || save->size == 0) return;

    uint8_t crc[CRC_SIZE];

    getcrc16(save->data + save->start + FRONTEND_START, FRONTEND_SIZE, crc);
    memcpy(save->data + save->start + FRONTEND_CRC_START, crc, CRC_SIZE);

    getcrc16(save->data + save->start + CAR_INFO_START, CAR_INFO_SIZE, crc);
    memcpy(save->data + save->start + CAR_INFO_CRC_START, crc, CRC_SIZE);

    getcrc16(save->data + save->start + TRACK_INFO_START, TRACK_INFO_SIZE, crc);
    memcpy(save->data + save->start + TRACK_INFO_CRC_START, crc, CRC_SIZE);

    getcrc16(save->data + save->start + TOURNAMENT_INFO_START, TOURNAMENT_INFO_SIZE, crc);
    memcpy(save->data + save->start + TOURNAMENT_INFO_CRC_START, crc, CRC_SIZE);

    getcrc16(save->data + save->start + RECORD_INFO_START, RECORD_INFO_SIZE, crc);
    memcpy(save->data + save->start + RECORD_INFO_CRC_START, crc, CRC_SIZE);

    printf("Fixed frontend CRC\n");
    printf("Fixed car info CRC\n");
    printf("Fixed track info CRC\n");
    printf("Fixed tournament info CRC\n");
    printf("Fixed record info CRC\n");
}

void updates(NFSHSSave* save, TYPE type, int32_t val[], char* str) {
    if(!save || !save->data || save->size == 0) return;

    switch(type) {
        case LANGUAGE: {
            if(!str) return;
            lower(str);

            int langidx = -1;
            for(int i = 0; i < LANGUAGES_SIZE; i++)
                if(strcmp(str, languages[i]) == 0) langidx = i;

            if(langidx > -1) {
                save->data[save->start + LANGUAGE_START] = (uint8_t) langidx;
                printf("Setted language: %s\n", str);
            }
            else return;

            break;
        }

        case CAR: {
            if(!val || !str) return;
            lower(str);

            int caridx = -1;
            for(int i = 0; i < CAR_COUNT; i++)
                if(cars[i] && strcmp(str, cars[i]) == 0) caridx = i;

            int upgidx = val[0] >= 0 && val[0] < UPGRADE_COUNT ? val[0] : -1;

            if(caridx > -1 && upgidx > -1) {
                int slotidx = -1;

                for(int i = 0; i < OWNED_CAR_SLOT_COUNT; i++) {
                    if(save->data[save->start + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * i] == 0xFF) {
                        slotidx = i;
                        break;
                    }
                }
                if(slotidx == -1) slotidx = 0;

                save->data[save->start + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * slotidx] = (uint8_t) caridx;
                save->data[save->start + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * slotidx + 1] = upgradedata[upgidx];
                save->data[save->start + CAR_INFO_START + OWNED_CAR_SLOT_SIZE * slotidx + 2] = (uint8_t) val[1];
                printf("Added car: slot %d | car %s | upgrade %d | color %d\n", slotidx, str, val[0], val[1]);
            }
            else return;

            break;
        }

        case CARS: {
            memset(save->data + save->start + CAR_AVAILABILITY_START, unlockedcardata, CAR_AVAILABILITY_SIZE);
            memset(save->data + save->start + CAR_VISIBILITY_START, unlockedcardata, CAR_VISIBILITY_SIZE);
            printf("Unlocked all cars\n");

            break;
        }

        case TRACKS: {
            memcpy(save->data + save->start + TRACK_INFO_START, alltracksdata, TRACK_INFO_SIZE);
            printf("Unlocked all tracks\n");

            break;
        }

        case MONEY: {
            if(!val) return;

            uint8_t buf[MONEY_SIZE];
            fromint(*val, buf);

            memcpy(save->data + save->start + TOURNAMENT_INFO_START, buf, MONEY_SIZE);
            printf("Setted money: %d\n", *val);

            break;
        }

        case TROPHIES: {
            memset(save->data + save->start + TROPHIES_START, goldtrophydata, TROPHIES_SIZE);
            printf("Setted gold trophies\n");

            break;
        }

    }

    fixs(save);
}

void writes(NFSHSSave* save) {
    FILE* fp = fopen(save->path, "wb");
    if(fp) {
        fwrite(save->data, sizeof(uint8_t), save->size, fp);
        fclose(fp);
        printf("Written on storage\n");
    }
}

#endif