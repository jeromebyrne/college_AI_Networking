#include "precompiled.h"
#include "NPC.h"
#include "Game.h"

NPC::NPC(float x , float y , int texture , float width , float height, int state, Maze * maze)
:GameObject2D(x,y,texture,width,height), m_currentState(state),m_closestTarget(0),m_lastTargetComputeTime(0),m_maze(maze),m_lastNodeComputeTime(0),
m_nodeDirection(AI_LEFT),m_wallEncountered(false),m_lastNodeDirection(AI_UP), m_defaultState(state),m_lastFireTime(0)
{
	m_health = 30;
}

NPC::~NPC(void)
{
}
void NPC::Update()
{
	// update base class
	GameObject2D::Update();
	
	// if greater then compute new target
	if(GetTickCount() > m_lastTargetComputeTime + COMPUTE_CLOSEST_TARGET_MAXTIME)
	{
		m_closestTarget = FindClosestTarget();
		m_lastTargetComputeTime = GetTickCount();
	}

	if(m_closestTarget != 0)
	{
		// get vector between us and target
		Vector2 vec = m_closestTarget->Position() - Position();

		float threshold = 150;
	
		// if target is within range
		if(vec.LengthSquared() < (threshold * threshold))
		{
			m_currentState = CHASE;
		
			// fire at the target
			if(Game::NumBullets() >= 500)
			{
				Game::ResetBullets();
			}
			if(GetTickCount() > m_lastFireTime + RATE_OF_FIRE_TIME)
			{
				FireProjectile(m_closestTarget->Position(),Game::NumBullets());
				Game::IncNumBullets();
				m_lastFireTime = GetTickCount();
			}
		}
		else
		{
			m_currentState = m_defaultState;
		}
	}

	switch(m_currentState)
	{
		case NONE:
			{
				break;
			}
		case PATROLING:
			{
				// if its time to calculate a new node to move to
				if(GetTickCount() > m_lastNodeComputeTime + COMPUTE_NEW_NODE_TIME)
				{
					Patrol();
					m_lastNodeComputeTime = GetTickCount();
				}
				break;
			}
		case WANDER:
			{
				// if its time to calculate a new node to move to
				if(GetTickCount() > m_lastNodeComputeTime + COMPUTE_NEW_NODE_TIME)
				{
					Wander();
					m_lastNodeComputeTime = GetTickCount();
				}
				break;
			}
		case SEARCHING:
			{
				if(m_maze != 0 /*&& m_closestTarget != 0*/)
				{
					A_StarSearch(m_closestTarget);
				}
				break;
			}
		case CHASE:
			{
				if(m_closestTarget != 0)
				{
					
					Chase(m_closestTarget);
				}
				break;
			}
		case EVADE:
			{
				break;
			}
	}

	// make the sprite spin... cool!!
	float xSpeed = VelocityX();
	float ySpeed = VelocityY();

	if(xSpeed < 0)
	{
		xSpeed = -xSpeed;
	}
	if(ySpeed < 0)
	{
		ySpeed = -ySpeed;
	}

	if(ySpeed <= xSpeed)
	{
		float value = Timing::GetInstance()->Elapsed() * 2;
		SetAngleY(GetAngleY() + VelocityY() * value);

	}
	else if(xSpeed < ySpeed)
	{
		float value = Timing::GetInstance()->Elapsed() * 2;
		SetAngleY(GetAngleY() - VelocityX() * value);
	}

}



/// chases the target parameter
void NPC::Chase(GameObject2D * target)
{
	Vector2 targetPos = target->Position();

	Vector2 direction = targetPos - m_position;

	float distanceSquared = direction.LengthSquared();

	direction.Normalise();

	if(distanceSquared > 30 * 30)
	{
		float moveSpeed = 5;
		SetVelocityX(VelocityX() + (direction.X * moveSpeed));
		SetVelocityY(VelocityY() + (direction.Y * moveSpeed));
	}

}
	
// avoid sthe target parameter
void NPC::Evade(GameObject2D * target)
{
}

// patrols a path
void NPC::Patrol()
{
	// find all adjacent tiles to our current tile
	list<MazeTile> adjacentTiles = m_maze->GetAdjacentTiles(m_position.X, m_position.Y);

	int size = adjacentTiles.size();

	// there should be 8 adjacent tiles
	if(size == 8)
	{
		list<MazeTile>::iterator current = adjacentTiles.begin();
		if(m_nodeDirection == AI_LEFT)
		{
			for(int i = 0; i < 7; i++)
			{
				current++;
			}
		}
		else if(m_nodeDirection == AI_UP)
		{
			current++;
		}
		else if(m_nodeDirection == AI_RIGHT)
		{
			for(int i = 0; i < 3; i++)
			{
				current++;
			}
		}
		else if(m_nodeDirection == AI_DOWN)
		{
			for(int i = 0; i < 5; i++)
			{
				current++;
			}
		}

		// if we are moving into a wall
		while(current->GetType() != NODE)
		{
			if(m_nodeDirection == AI_LEFT)
			{
				current = adjacentTiles.begin();
				current++;

				m_nodeDirection = AI_UP;
			}
			else if(m_nodeDirection == AI_UP)
			{
				current = adjacentTiles.begin();

				for(int i = 0; i < 3; i++)
				{
					current++;
				}
				m_nodeDirection = AI_RIGHT;
			}
			else if(m_nodeDirection == AI_RIGHT)
			{
				current = adjacentTiles.begin();

				for(int i = 0; i < 5; i++)
				{
					current++;
				}
				m_nodeDirection = AI_DOWN;
			}
			else if(m_nodeDirection == AI_DOWN)
			{
				current = adjacentTiles.begin();

				for(int i = 0; i < 7; i++)
				{
					current++;
				}
				m_nodeDirection = AI_LEFT;
			}
			
		}

		
		// get vector to top right
		Vector2 direction = (*current).m_centrePos - m_position;
		direction.Normalise();

		float moveSpeed = 70;

		SetVelocityX(direction.X * moveSpeed);
		SetVelocityY(direction.Y * moveSpeed);
	}
}

// wanders randomly
void NPC::Wander()
{
	// find all adjacent tiles to our current tile
	list<MazeTile> adjacentTiles = m_maze->GetAdjacentTiles(m_position.X, m_position.Y);

	int size = adjacentTiles.size();

	// there should be 8 adjacent tiles
	if(size == 8)
	{
		// up tile
		list<MazeTile>::iterator up = adjacentTiles.begin();
		up++;
		
		// right tile
		list<MazeTile>::iterator right = adjacentTiles.begin();
		for(int i = 0; i < 3; i++)
		{
			right++;
		}

		//down tile
		list<MazeTile>::iterator down = adjacentTiles.begin();
		for(int i = 0; i < 5; i++)
		{
			down++;
		}

		//left tile
		list<MazeTile>::iterator left = adjacentTiles.begin();
		for(int i = 0; i < 7; i++)
		{
			left++;
		}
		
		Vector2 direction = m_velocity;

		direction.Normalise();

		// move up
		MazeTile destination = (*up);

		// we are moving right in relation to the map then we will try to move up
		if(direction.X > 0.7)
		{
			destination = *up;

			// if always wanna move left
			if(destination.GetType() != NODE)
			{
				destination = *right;

				if(destination.GetType() != NODE)
				{
					destination = *down;
					if(destination.GetType() != NODE)
					{
						destination = *left;
					}
				}
			}
		}
		else if(direction.X < -0.7)
		{
			destination = *down;

			// if always wanna move left
			if(destination.GetType() != NODE)
			{
				destination = *left;

				if(destination.GetType() != NODE)
				{
					destination = *up;

					if(destination.GetType() != NODE)
					{
						destination = *right;
					}
				}
			}
		}
		if(direction.Y > 0.7)
		{
			destination = *right;

			// if always wanna move left
			if(destination.GetType() != NODE)
			{
				destination = *down;

				if(destination.GetType() != NODE)
				{
					destination = *left;

					if(destination.GetType() != NODE)
					{
						destination = *up;
					}
				}
			}
		}
		else if(direction.Y < -0.7)
		{
			destination = *left;

			// if always wanna move left
			if(destination.GetType() != NODE)
			{
				destination = *up;

				if(destination.GetType() != NODE)
				{
					destination = *right;

					if(destination.GetType() != NODE)
					{
						destination = *down;
					}
				}
			}
		}

		
		// get vector to top right
		Vector2 vec = destination.m_centrePos - m_position;
		vec.Normalise();

		float moveSpeed = 170;

		SetVelocityX(vec.X * moveSpeed);
		SetVelocityY(vec.Y * moveSpeed);
	}
}
// blindly search for a target
void NPC::BlindSearch(GameObject2D * target)
{
}

// you know the position of the target
void NPC::A_StarSearch(GameObject2D * target)
{
	
}

// finds the closest target to the 
GameObject2D * NPC:: FindClosestTarget()
{
	GameObject2D * target = 0;
	float smallestDistance = 99999999.9f;

	map<int,GameObject2D *>::iterator current = Game::GameObjectMap().begin();
	map<int,GameObject2D *>::iterator end = Game::GameObjectMap().end();

	// iterate through 
	for(; current != end; current++)
	{
		// only check player characters (port 28000+)

		if((*current).first >= 28000 && current->second != 0)
		{
			// check distance
			float distance = m_position.Distance((*current).second->Position());
			if(distance < smallestDistance)
			{
				smallestDistance = distance;
				target = (*current).second;
			}
		}

	}

	return target;
}
