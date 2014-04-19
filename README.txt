***NETWORKING + AI PROJECT***

REQUIRED TO RUN EXE: none

REQUIRED TO COMPILE: Visual Studio 2005/2008

I have provided both the debug executable and the release executable.
There are only minor differences in the these. However, release executable runs faster so I 
recommend running this when testing.

The visual studio solution is divided into 2 projects, one for the client and one for the server.
ALL AI CODE IS IN THE SERVER PROJECT (NetAI_Server).

AI code can be found in AI/NPC.h and AI/NPC.cpp. There is also code related to AI in game.cpp and node navigation uses code which is found in Maze.cpp.

ORDER OF EXECUTION:

1: double click server exe.
	here you will see the AI entities navigating the map

2: double click client exe.
	you will appear in the world (you can see yourself on the server too, ID will most likely be 28000)

3: You can click on the client exe again to add more clients (ID will increase by 1, so next client will be 28001 on server screen)
	NOTE: Adding a lot of clients will obviously impact on performance.

NOTE: You can run the clients before the server if you so wish but you wont appear on the screen and you cannot 
      interact with anything (apart from moving aound the map) until the server is created

					***HOW TO PLAY***
MOVE UP: 	'W'
MOVE DOWN:	'S'
MOVE LEFT:	'A'
MOVE RIGHT:	'D'

FIRE BULLET: Just click on the screen wherever you want to fire, your entity will fire towards this point.
		NOTE: Bullets appear to be firing a good bit on front of the ship, this is due to the time it takes to make a fire request 
			to the server and receive validation.

Player starts with 100 health.
NPC's start with 30 health.

The longer a bullet stays within an entities bounding box the more damage it does, so just skimmimg an enemy will do less damage than sending a bullet right through them.

				***MINOR ISSUES***

Bullets bounce off walls which can be strategically used to eliminate enemies.
	NOTE: bullets are processed on the server and predicted on the client side so you will see snapping of bullets when they hit the walls.

Because the client predicts their position, this may differ on the server and some slight snapping may appear, this will occur more often when 
the server is under heavy strain i.e Lots of entites in the same area at once. 

NPC's can kill each other and themselves with their own bullets (rebounding). Changing this required multiple changes to both the server and the client
 and I felt my time was best spent on other more important features. The player can also damage himself by rebounding bullets off walls.


					***AI***

The AI entities are initially wandering or patroling. When patroling they will move around a certain area of the map. 
When wandering they will move around the map by constantly trying to move to their left.

When an NPC comes within range of the player they will dynamically change their state to pursue and attack (fire bullets).
The NPC always actively pursues the closest target to them (can test with multiple clients). 
When the NPC falls back out of range they will go back to their previous state.

When an NPC has caught wind of a player they will call for backup. Any other NPC within a certain range of him will be attracted and form a squad,
then the attracted squad member will eventualy come within range of the player and change their state to pursue and attack, this will in turn cause the NPC to call for more backup.
Each member of the squad will push apart from each other and the player and in turn try to surround the player.

STATE BASED BEHAVIOUR AND NAVIGATION CAN BE FOUND IN 'NPC.H' and 'NPC.CPP'.
Squad behaviour and separation can be found in the DeCluster() function which is found in game.cpp in the server project.
To find adjacent tiles for navigation, see - 'maze.cpp' on the server project





***************** IF YOU HAVE ANY PROBLEMS OR WANT CLARIFICATION ON CERTAIN ASPECTS LET ME KNOW ***************************************
