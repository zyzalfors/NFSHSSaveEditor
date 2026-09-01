#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "NFSHSSave.h"

#define PRINT_HELP "-help"
#define PRINT_CMD "-print"
#define FIX_CMD "-fix"
#define SET_LANGUAGE_CMD "-lang"
#define ADD_CAR_CMD "-car"
#define UNLOCK_CARS_CMD "-cars"
#define UNLOCK_TRACKS_CMD "-tracks"
#define SET_MONEY_CMD "-money"
#define SET_GOLD_TROPHIES_CMD "-trophies"

void printhelp() {
    printf(
        "Usage: NFSHSaveEditor <command>\n"
        "Commands:\n"
        "-help                                                               Show this help screen\n"
        "-print [filepath] [start offset]                                    Print save\n"
        "-fix [filepath] [start offset]                                      Fix save checksums\n"
        "-cars [filepath] [start offset]                                     Unlock all cars\n"
        "-tracks [filepath] [start offset]                                   Unlock all tracks.\n"
        "-trophies [filepath] [start offset]                                 Unlock all gold trophies\n"
        "-lang [filepath] [start offset] [lang]                              Set game language. Set [lang] among {en, ge, fr, sp, it, sw}\n"
        "-money [filepath] [start_offset] [money]                            Set amount of money\n"
        "-car [filepath] [start_offset] [car name] [upgrade level] [color]   Add a car to first free slot. Set [car name] among {slk, z3, hsvvt, falcon, camaro, firebird, db7, xkr, m5, corvette, 550, 911, f50, diablo, clk, f1, race_911, race_corvette, phantom, titan, cop_caprice, cop_hsvvt, cop_m5, cop_corvette, cop_911, cop_diablo, jailbird}. Set [upgrade level] among {0, 1, 2, 3}. Set [color] among {0, 1,...}\n"
    );
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 2: {
            if(strcmp(argv[1], PRINT_HELP) == 0) printhelp();
            break;
        }

        case 4: {
            NFSHSSave save;
            inits(&save, argv[2], strtol(argv[3], NULL, 10));

            if(strcmp(argv[1], PRINT_CMD) == 0)
                prints(&save);
            else if(strcmp(argv[1], FIX_CMD) == 0) {
                fixs(&save);
                writes(&save);
            }
            else if(strcmp(argv[1], UNLOCK_CARS_CMD) == 0) {
                updates(&save, CARS, NULL, NULL);
                writes(&save);
            }
            else if(strcmp(argv[1], UNLOCK_TRACKS_CMD) == 0) {
                updates(&save, TRACKS, NULL, NULL);
                writes(&save);
            }
            else if(strcmp(argv[1], SET_GOLD_TROPHIES_CMD) == 0) {
                updates(&save, TROPHIES, NULL, NULL);
                writes(&save);
            }

            clears(&save);
            break;
        }

        case 5: {
            NFSHSSave save;
            inits(&save, argv[2], strtol(argv[3], NULL, 10));

            if(strcmp(argv[1], SET_LANGUAGE_CMD) == 0) {
                updates(&save, LANGUAGE, NULL, argv[4]);
                writes(&save);
            }
            else if(strcmp(argv[1], SET_MONEY_CMD) == 0) {
                int32_t val = strtol(argv[4], NULL, 10);
                updates(&save, MONEY, &val, NULL);
                writes(&save);
            }

            clears(&save);
            break;
        }

        case 7: {
            NFSHSSave save;
            inits(&save, argv[2], strtol(argv[3], NULL, 10));

            if(strcmp(argv[1], ADD_CAR_CMD) == 0) {
                int32_t val[2] = {strtol(argv[5], NULL, 10), strtol(argv[6], NULL, 10)};
                updates(&save, CAR, val, argv[4]);
                writes(&save);
            }

            clears(&save);
            break;
        }
    }
}