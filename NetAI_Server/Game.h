#ifndef GAME_H
#define GAME_H

#include "Projectile.h"
#include "Player.h"
#include "Maze.h"
#include "NPC.h"

#define MAX_PLAYERS 8 // the max number of players that can join

#define SEND_TIME 50 // send out packet every n milliseconds

class Game
{
private:
	BasicCamera * camera;

	Maze * m_maze;

	// the texture used to display players
	int playerTexture;

	int m_packetsReceived;// number of packets recieved from clients

	int m_lastPacketSentTime; // the last time we sent our packets
	
	// this keeps hold of all game objects on the server
	static map<int,GameObject2D*> m_gameObjectMap;

	// the number of clients that are playing 
	static int m_numPlayersJoined;

	// a list of all the npc's
	list<NPC *> m_npcList;

	// the number of bullets in the world
	static int numBullets;

public:
	Game(void);
	~Game(void);
	
	static void AddGameObject2D(GameObject2D * object, int ID);
	static bool RemoveGameObject2D(int ID);
	void Initialise();
	void Update(Vector2 mouse);
	void Draw();
	
	// returns the number of the next bullet in the world
	static int NumBullets();

	// increments the number of bullets
	static void IncNumBullets();

	// resets bullet count back to 0
	static void ResetBullets();

	// receive data from clients
	void ReceiveData();

	// send a packet to a port number
	void SendData();
	
	// draws a simple text hud
	void DrawHud();

	// check if bullets are hitting entities and respond
	void CheckHits();

	// stops the ai from sticking to each other
	void DeClusterAI();

	// returns the map of game objects
	static map<int,GameObject2D*> & GameObjectMap()
	{
		return m_gameObjectMap;
	}

};

#endif
