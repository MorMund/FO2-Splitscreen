# FlatOut 2 Splitscreen Mod
![gameplay](http://imgur.com/wNrbZ8I.png)

### 1. Installation
 1. Install [Microsoft .NET Framework 4.5.2](https://www.microsoft.com/en-us/download/details.aspx?id=42643).
 2. Download the latest release at [Releases](https://github.com/DeadlySurprise/FO2-Splitscreen/releases).
 3. Extract all files into the FlatOut 2 game folder. (e.g. C:\\Program Files\\FlatOut2\\).  
 3a. If you're using a fresh installation of the game, launch it once regularly. (Run FlatOut2.exe)
 4. Run the Launcher.exe

### 2. Configuration
![Launcher configuration](http://i.imgur.com/0BcnTTi.png)  

1. General Tab  
  * Select number of players
  * Set resolution of the game windows
 
2. Controllers tab
  * Select a controller for each player

3. In-game  
**All In-game setup can only be performed by using the keyboard and clicking on the window of the instance you're trying to setup.**
 * Go to settings, controller options and select your controller. Afterwards you can change your button bindings. These settings are usually saved between sessions.
 * Now you can start a multiplayer session by selecting multiplayer -> LAN in the main menu and selecting Create game with the host instance (The top left game.)
 * Afterwards you can join with all other players.  
**Even though you are playing in LAN mode it is currently not possible to join a splitscreen session from another computer in your network!**  
4. Advanced Settings  
 * Settings can be changed manually by editing `Splitscreen_Settings.xml` and starting the `Launcher.exe` with the command line argument `-noConfig`  

### 3. Troubleshooting :
 * If the game repeatedly crashes on startup, try disabling "Skip intro videos" in the launcher.
