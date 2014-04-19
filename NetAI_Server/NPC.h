#ifndef NPC_H
#define NPC_H

#include "gameobject2d.h"
#include "Maze.h"

#define COMPUTE_CLOSEST_TARGET_MAXTIME 2000 // the time in ms to compute the closest player character to this NPC

#define COMPUTE_NEW_NODE_TIME 250 // the time between calculating a new node to move to

#define RATE_OF_FIRE_TIME 2000 // the time in between firing

class NPC : public GameObject2D
{
private:

	// holds a pointer to the maze
	Maze * m_maze;

	// current state
	int m_currentState;

	// the state in which the NPC started in and defaults back to
	int m_defaultState;
	
	// the closest player character to the NPC
	GameObject2D * m_closestTarget;

	// the last time we computed closest target
	int m_lastTargetComputeTime;

	// the last time we computed a new node to move to
	int m_lastNodeComputeTime;

	// the last time the NPC fired
	int m_lastFireTime;

	// have we encountered a wall while navigating
	bool m_wallEncountered;

	// the direction in which the player is moving to a node
	int m_nodeDirection;

	// the direction the npc was moving
	int m_lastNodeDirection;

	/// chases the target parameter
	virtual void Chase(GameObject2D * target);
	
	// avoid sthe target parameter
	virtual void Evade(GameObject2D * target);

	// patrols a path
	virtual void Patrol();
	
	// wanders randomly
	virtual void Wander();

	// blindly search for a target
	virtual void BlindSearch(GameObject2D * target);

	// you know the position of the target
	virtual void A_StarSearch(GameObject2D * target);

	// finds the closest target to the 
	virtual GameObject2D * FindClosestTarget();



public:
	NPC(float x = 0, float y = 0, int texture = 0, float width = 1, float height = 1, int state = PATROLING, Maze * maze = 0);
	virtual ~NPC(void);
	virtual void Update();

	// returns the AI's current state
	inline int CurrentState()
	{
		return m_currentState;
	}
	
};

#endif
