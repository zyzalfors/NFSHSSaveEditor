#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "NFSHSSaveEditor.h"

#define PRINT_HELP "-help"
#define PRINT "-print"
#define FIX "-fix"
#define SET_LANGUAGE "-lang"
#define SET_CAR "-car"
#define UNLOCK_CARS "-cars"
#define UNLOCK_TRACKS "-tracks"
#define SET_MONEY "-money"
#define SET_GOLD_TROPHIES "-trophies"

void printhelp() {
    printf(
        "Usage: NFSHSaveEditor <command>\n"
        "Commands:\n"
        "-help                                                Show this help screen\n"
        "-print [filepath]                                    Print save(s)\n"
        "-fix [filepath]                                      Fix save(s) checksums\n"
        "-cars [filepath]                                     Unlock all cars\n"
        "-tracks [filepath]                                   Unlock all tracks\n"
        "-trophies [filepath]                                 Unlock all gold trophies\n"
        "-lang [filepath] [lang]                              Set language [lang] among {en, de, fr, sp, it, sw}\n"
        "-money [filepath] [money]                            Set money\n"
        "-car [filepath] [car name] [upgrade level] [color]   Set car to first free slot. Set [car name] among {slk, z3, hsvvt/skyline, falcon, camaro, firebird, db7, xkr, m5, corvette, 550, 911, f50, diablo, clk, f1, race_911, race_hsvvt/race_skyline, race_corvette, phantom, titan, cop_caprice, cop_hsvvt, cop_m5, cop_corvette, cop_911, cop_diablo, jailbird}. Set [upgrade level] among {0, 1, 2, 3}. Set [color] among {0, 1,...}\n"
    );
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 2: {
            if(strcmp(argv[1], PRINT_HELP) == 0) printhelp();
            break;
        }

        case 3: {
            if(strcmp(argv[1], PRINT) == 0) {
                NFSHSSaveEditor editor = init(argv[2]);
                print(&editor);
                clear(&editor);
            }
            else if(strcmp(argv[1], FIX) == 0) {
                NFSHSSaveEditor editor = init(argv[2]);
                fix(&editor);
                save(&editor);
                clear(&editor);
            }
            else if(strcmp(argv[1], UNLOCK_CARS) == 0) {
                NFSHSSaveEditor editor = init(argv[2]);
                update(&editor, CARS, NULL, NULL);
                save(&editor);
                clear(&editor);
            }
            else if(strcmp(argv[1], UNLOCK_TRACKS) == 0) {
                NFSHSSaveEditor editor = init(argv[2]);
                update(&editor, TRACKS, NULL, NULL);
                save(&editor);
                clear(&editor);
            }
            else if(strcmp(argv[1], SET_GOLD_TROPHIES) == 0) {
                NFSHSSaveEditor editor = init(argv[2]);
                update(&editor, TROPHIES, NULL, NULL);
                save(&editor);
                clear(&editor);
            }

            break;
        }

        case 4: {
            if(strcmp(argv[1], SET_LANGUAGE) == 0) {
                NFSHSSaveEditor editor = init(argv[2]);
                update(&editor, LANGUAGE, NULL, argv[3]);
                save(&editor);
                clear(&editor);
            }
            else if(strcmp(argv[1], SET_MONEY) == 0) {
                NFSHSSaveEditor editor = init(argv[2]);
                int32_t val = strtol(argv[3], NULL, 10);
                update(&editor, MONEY, &val, NULL);
                save(&editor);
                clear(&editor);
            }

            break;
        }

        case 6: {
            if(strcmp(argv[1], SET_CAR) == 0) {
                NFSHSSaveEditor editor = init(argv[2]);
                int32_t val[2] = {strtol(argv[4], NULL, 10), strtol(argv[5], NULL, 10)};
                update(&editor, CAR, val, argv[3]);
                save(&editor);
                clear(&editor);
            }

            break;
        }
    }
}
