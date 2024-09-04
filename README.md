# UE_ActionRoguelike
This repository contains the game that I made in Unreal Engine while following the Tom Looman's "Professional Game Development in C++ and Unreal Engine" course. The game contains all the features that were taught in the course as well as my own features and improvements.

## Feature Highlights

### Features that were taught in the course:

* Players can control a character that moves, does actions and interacts with the environment. The character is split in multiple components so that the SCharacter class doesn't become cluttered.
    - The character, named SCharacter in the source code, has an SActionComponent as a member variable. SActionComponent controls all actions a Pawn in the game would do. To give a Pawn an action or effect (SAction), it needs to be added to their SActionComponent. The          SActionComponent will check if the Pawn is allowed to own this action by checking it against blocking GameplayTags (e.g. Gameplaytag "sprinting" would block PrimaryAttack). The SActionComponent also replicates all actions from the Pawn for 
      clients in multiplayer.
    - There is also a SAttributeComponent that controls the health and rage of Pawns. It will check if Pawns have enough rage to perform actions, and makes sure that Pawns die when their health is equal or below zero.
    - Adding SInteractionComponent to an ACharacter allows them to interact with Actors that have inherited the SGameplayInterface. A popup, with customizable text for each Actor, will appear above the Actor if the character points their crossair towards it. Once   
      interacted, the Actor can perform various actions (grant coins, open lid of chest, etc.).
* SCharacter and SMinions have a Widget Blueprint that shows their health and icons of SActionEffects that they have with a timer indicating how much time is left till the effect stops. The player itself has a HUD that also shows their rage and in-game time.
* SMinions have a basic AI that allows them to detect players, shoot, heal and hide behind cover.
    - Minions are spawned in as the game progresses, dependent on a curve that determines the maximum amount of minions at a given time.
    - A Behaviour Tree determines a minion's actions. Multiple variables determine what a Minion should do (e.g. if the Minion is low health or if it is within Attack Range of a player). Once the best action is determined, a BTTask will perform the action.
* Powerups are spawned throughout the map with an Environment Query System. The Spawn Rate, Respawn Timer and Credit Cost of each Powerup can be adjusted.
* There is a saving and loading system where players can save their game state by interacting with an in-game save point. Saving will save the player's credits and transformation as well as the transformation of all other Actors.
* Multiplayer Support exists for an arbitrary number of clients. One player can host a game from the main menu, and others can join using the IP-Adress of the Host. The entire game state is synchronized and replicated across clients. Important variables like health, rage and actions are stored on the server to prevent race conditions.
* There are various Console Commands to help with development. E.g. "ar.SpawnBots" can toggle the spawning of AI-Controlled minions and "God" makes the player invulnerable.

### Features and Improvements that I have made on my own:

* Players can make and select save files from the main menu. Once the main menu is loaded, the game will search for present save files. It will add the names of these files to a ComboBox. The player can select which of these saves it wants to load. Furthermore, a player can choose to name and make a new save.
* Abilities are loaded on demand by using TSoftClassPtr's in SAction_ProjectileAttack using LoadProjectileClass(). When an ability is used, it is being loaded at that time, instead of at the game start. This is done to decrease initial load times. On abilities where there is a "wind up time", the ability is also being loaded in an asynchronous matter on a different thread.

## Credits
The source code of the game is both my own and from the [code provided by this course](https://github.com/tomlooman/ActionRoguelike/branches).

Game Assets: Licensed for use with the Unreal Engine only. Without a custom license you cannot use to create sequels, remasters, or otherwise emulate the original game or use the original game’s trademarks, character names, or other IP to advertise or name your game. (Unreal Engine EULA applies) (Please note this applies to the Game Assets that refer to Epic's Paragon)

Created in Unreal Engine 5.3.2
