# Need for Speed High Stakes/Road Challenge Save Editor
This GUI tool allows users to read and edit save files for the PlayStation game Need for Speed High Stakes/Road Challenge-Over Drivin' IV. It supports all save regions.

It supports the formats RAW (raw save bytes), SC (PS1 save slot), MC (PS1 memory card image), GME (DexDrive), and PSV (PS1 save image for PS3), but it does not support resigning PSV files.

The tool can edit various properties such as language, money, and owned vehicles, as well as unlock cars, tracks, and gold trophies.

Research into save editing has allowed me to discover some interesting details about the game:
* The language can be changed to any supported language, even if it is not officially listed in that game version. While this allows the text to be correctly translated, voice car descriptions will be absent. I am not sure if this applies to the Japanese (JA) version of the game
* Obviously, money is stored as a signed integer in the save data, so it is possible to set an amount significantly higher than the visual cap of 999999999
* Players can add any car listed in the combobox to their owned-car slots. In particular, this allows them to obtain normally unobtainable vehicles such as the upgraded CLK GTR and F1 GTR, police cars and their upgraded variants, and even the Jailbird helicopter and traffic vehicles
* If one enters AU/JP cars into EU/US save files, games often allow users to select these cars, but only an SLK placeholder appears on-screen. In other cases, this can cause the game to crash on the car selection screen. I suspect similar issues may also occur with AU/JP saves. Similar problems can happen if traffic cars are added
* The most curious fact is that if a police car is added to the owned-car slots and used in a race, the game is still playable, although with some weird behavior, since the game is fooled into believing that the race is a pursuit