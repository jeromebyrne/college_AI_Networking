#ifndef GAME_H
#define GAME_H

#include "Projectile.h"
#include "Player.h"
#include "Maze.h"
#include "AOImanager.h"

#define PACKET_SEND_TIME 50 // time in milliseconds, when to send a packet

#define MAX_BULLETS 500 // the maximum number of bullets in the world at any one time

class Game
{
private:
	BasicCamera * camera;

	// this keeps hold of all game objects on the client side
	static map<int,GameObject2D*> m_gameObjectMap;

	Player * m_player;  // this client
	Maze * m_maze;
	AOImanager * m_aoiManager;

	int playerTexture;
	int npcTexture;
	int npcTexture2;
	int m_lastSendPacketTime;

	int m_numBullets; // the current number of bullets in the world

public:

	static Vector2 m_mousePos; // store the mouse coordinates here

	Game(void);
	~Game(void);
	
	static void AddGameObject2D(GameObject2D * object,int ID);
	static bool RemoveGameObject2D(int ID);
	void Initialise();
	void Update(Vector2 mouse);
	void Draw();
	
	// draws a simple text hud
	void DrawHud();

	// receive network data for all clients in the game
	void ReceiveData();

};

#endif
