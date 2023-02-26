**Hitman 3 Epic-Steam Achievements**

This is a project I made to transfer any Epic Games Store achievements for HITMAN 3 to the Steam release of HITMAN 3.

**INSTALLING**

NOTES:
 - You require the Epic Games Store release of HITMAN 3 to use this. You must also own the Steam release, but having it installed this not required.
 - `dinput8.dll` will also load ZHMModSDK if present in the HITMAN 3 install directory.

INSTALLATION INSTRUCTIONS:

 - Download the latest release from the releases tab.
 - Place `dinput8.dll`, `AchievementHandler.dll`, `steam_api64.dll` and `steam_appid.txt` into the HITMAN3/Retail folder.
 - Once that is done, you can launch the game like normal.

UNINSTALLATION INSTRUCTIONS:
 - Delete `dinput8.dll`, `AchievementHandler.dll`, `steam_api64.dll` and `steam_appid.txt` from the HITMAN3/Retail folder.

**BUILDING**
- Place the Epic Games Store SDK into `External/EOSSDK` so that the folder structure looks like:
    
        External/EOSSDK/Bin
        External/EOSSDK/Include
        External/EOSSDK/Lib
        External/EOSSDK/Tools

- Next, do the same with the Steamworks SDK so that the folder structure looks like:

        External/steam/sdk/public
        External/steam/sdk/redistributable_bin
