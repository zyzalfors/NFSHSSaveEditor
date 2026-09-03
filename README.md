# Need for Speed High Stakes/Road Challenge Save Editor
This command-line tool allows users to read and edit save files for the PlayStation game Need for Speed High Stakes/Road Challenge. It supports US, EU, AU saves, with some compatibility for JP saves. It supports any format, such as .gme or .psv, containing save data.
```
The usage is:
-help                                                               Show this help screen
-print [filepath] [start offset]                                    Print save
-fix [filepath] [start offset]                                      Fix save checksums
-cars [filepath] [start offset]                                     Unlock all cars
-tracks [filepath] [start offset]                                   Unlock all tracks
-trophies [filepath] [start offset]                                 Unlock all gold trophies
-lang [filepath] [start offset] [lang]                              Set game language. Set [lang] among {en, ge, fr, sp, it, sw}
-money [filepath] [start offset] [money]                            Set amount of money
-car [filepath] [start offset] [car name] [upgrade level] [color]   Add a car to first free slot. Set [car name] among {slk, z3, hsvvt, falcon, camaro, firebird, db7, xkr, m5, corvette, 550, 911, f50, diablo, clk, f1, race_911, race_corvette, phantom, titan, cop_caprice, cop_hsvvt, cop_m5, cop_corvette, cop_911, cop_diablo, jailbird}. Set [upgrade level] among {0, 1, 2, 3}. Set [color] among {0, 1,...}
```

To keep the editor logic simple, the user must enter the start offset of the save data within the file. The save data begins with 4 bytes that always contain its size (5292), stored in little-endian order. Any bytes before the start of the save data or after its end are ignored. The start offset can be determined using a hex editor.

Research into save editing has allowed me to discover some interesting details about the game:
* The language can be changed to any of the supported languages, even if that language is not listed as available in the game version. Changing to an otherwise unavailable language allows the text to be correctly translated, but voice car descriptions are absent
* Obviously, money is stored as a signed integer in the save data, so it is possible to set an amount significantly higher than the visual cap of 999999999
* Do not enter AU cars (hsvvt, falcon) in EU/US saves, as this can cause the game to crash on the car selection screen because the corresponding car data is absent from non-AU builds. I suspect the same issue may also occur with the JP version
* Using the appropriate command, one can add the upgraded versions of the CLK-GTR and F1 GTR to the owned-car slots, even though they cannot normally be obtained
* The most curious fact is that any car listed in the usage can be added to the owned-car slots, including police cars and the Jailbird helicopter! If a police car is added and used in a race (either a single race or a tournament), the game appears to behave correctly, but it may crash in some cases
