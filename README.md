# Need for Speed High Stakes/Road Challenge Save Editor
This GUI tool allows users to read and edit save files for the PlayStation game Need for Speed High Stakes/Road Challenge. It supports US, EU, AU, JA saves.

It supports the formats RAW (raw save bytes), SC (PS1 save slot), MC (PS1 memory card image), GME (DexDrive), PSV (PS1 save image for PS3).

The tool can edit various properties such as language, money, owned cars, and can unlock cars, tracks, and gold trophies.

Research into save editing has allowed me to discover some interesting details about the game:
* The language can be changed to any of the supported languages, even if that language is not listed as available in the game version. Changing to an otherwise unavailable language allows the text to be correctly translated, but voice car descriptions are absent
* Obviously, money is stored as a signed integer in the save data, so it is possible to set an amount significantly higher than the visual cap of 999999999
* Do not enter AU/JP cars in EU/US saves, as this can cause the game to crash on the car selection screen because the corresponding car data is absent from non-AU/JP builds. I suspect the similar issues may also occur with AU/JP saves
* One can add any car listed in the usage to the owned-car slots. In particular, one can obtain normally unobtainable cars, such as the upgraded CLK GTR and F1 GTR, police cars and their upgraded versions, and even the Jailbird helicopter
* The most curious fact is that if a police car is added to the owned-car slots and used in a race, the game is still playable, although with some weird behavior, since the game is fooled into believing that the race is a pursuit