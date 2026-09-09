# Need for Speed High Stakes/Road Challenge Save Editor
This command-line tool allows users to read and edit save files for the PlayStation game Need for Speed High Stakes/Road Challenge. It supports US, EU, AU, JP saves.

It supports the formats RAW (raw save bytes), SC (PS1 save slot), MC (PS1 memory card image), GME (DexDrive), PSV (PS1 save image for PS3).

```
Usage: NFSHSaveEditor <command>
Commands:
-help                                                Show this help screen
-print [filepath]                                    Print save(s)
-fix [filepath]                                      Fix save(s) checksums
-cars [filepath]                                     Unlock all cars
-tracks [filepath]                                   Unlock all tracks
-trophies [filepath]                                 Unlock all gold trophies
-lang [filepath] [lang]                              Set language [lang] among {en, ge, fr, sp, it, sw}
-money [filepath] [money]                            Set money
-car [filepath] [car name] [upgrade level] [color]   Set car to first free slot. Set [car name] among {slk, z3, hsvvt/skyline, falcon, camaro, firebird, db7, xkr, m5, corvette, 550, 911, f50, diablo, clk, f1, race_911, race_hsvvt/race_skyline, race_corvette, phantom, titan, cop_caprice, cop_hsvvt, cop_m5, cop_corvette, cop_911, cop_diablo, jailbird}. Set [upgrade level] among {0, 1, 2, 3}. Set [color] among {0, 1,...}
```

Research into save editing has allowed me to discover some interesting details about the game:
* The language can be changed to any of the supported languages, even if that language is not listed as available in the game version. Changing to an otherwise unavailable language allows the text to be correctly translated, but voice car descriptions are absent
* Obviously, money is stored as a signed integer in the save data, so it is possible to set an amount significantly higher than the visual cap of 999999999
* Do not enter AU/JP cars (hsvvt/skyline, falcon) in EU/US saves, as this can cause the game to crash on the car selection screen because the corresponding car data is absent from non-AU/JP builds. I suspect the similar issues may also occur with AU/JP saves
* Using the appropriate command, one can add any car listed in the usage to the owned-car slots. In particular, one can obtain normally unobtainable cars, such as the upgraded CLK GTR and F1 GTR, police cars and their upgraded versions, and even the Jailbird helicopter
* The most curious fact is that if a police car is added to the owned-car slots and used in a race, the game is still playable, although with some weird behavior, since the game is fooled into believing that the race is a pursuit