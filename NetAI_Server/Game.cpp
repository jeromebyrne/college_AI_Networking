#include "precompiled.h"
#include "Game.h"
#include "BitPacker.h"

// initialise static map
map<int,GameObject2D *> Game::m_gameObjectMap;

int Game::numBullets = 0;

int Game::m_numPlayersJoined = 0;

Game::Game(void):m_packetsReceived(0),m_lastPacketSentTime(0)
{
	camera = new BasicCamera();
}

Game::~Game(void)
{
	// traverse the list of all game objects and delete them
	//TODO: delete objects

	if(m_maze != 0)
	{
		delete m_maze;
		m_maze = 0;
	}
	
}

// check if bullets are hitting entities and respond
void Game::CheckHits()
{
	map<int,GameObject2D*>::iterator current = m_gameObjectMap.begin();

	// traverse the object map 
	for(;current != m_gameObjectMap.end(); current++)
	{
		int ID = current->first;
		
		// check each projectile
		if(ID >= 0 && ID <= 500 && current->second != 0)
		{
				map<int,GameObject2D*>::iterator other = m_gameObjectMap.begin();

				// traverse the object map
				for(;other != m_gameObjectMap.end(); other++)
				{
					int otherID = other->first;

					// check each entities
					if(otherID > 500 && other->second != 0)
					{
						if(
							   other->second->Right() + 10 > current->second->X()	&&
							   other->second->Left() - 10 < current->second->X()		&&
							   other->second->Top() + 10 > current->second->Y()		&&
							   other->second->Bottom() - 10 < current->second->Y()
						   )
						{
							// we have a collision
							other->second->DecrementHealth(0.1);

							if(other->second->Health() <= 0)
							{
								
								RemoveGameObject2D(other->first);
	
							}
						}
					}

				}// end of inner for
		}

	}//end of outer for
}

int Game::NumBullets()
{
	return numBullets;
}
// increments the number of bullets
void Game::IncNumBullets()
{
	numBullets++;
}
// resets bullet count back to 0
void Game::ResetBullets()
{
	numBullets = 0;
}

void Game::Initialise()
{
	// initialise our texture manager
	Texture::GetInstance()->Initialise(GLwindow::GetInstance()->GetHwndRef());
	
	// texture for the player sprite
	playerTexture = Texture::GetInstance()->generateTexture("textures/player.bmp");
	
	// create a new maze
	m_maze = new Maze(0,0,60);
	m_maze->Build();

	srand(GetTickCount()); // seed 
	
	// create a new network connection
	Net::GetInstance()->SetUpConnection(25000);
	

	// add some patroling and wandering NPC's

	for(int i = 1; i <= 10; i++)
	{
		int posX = (rand()%2200) + 50;
		int posY = (rand()%900) + 50;

		NPC * npc = new NPC(posX,posY,0,15,15,PATROLING,m_maze);
		AddGameObject2D(npc,500 + i);
		
		// add to the npc list
		m_npcList.push_back(npc);
	}
	for(int i = 10; i <= 20; i++)
	{
		int posX = (rand()%2200) + 50;
		int posY = (rand()%900) + 50;

		NPC * npc = new NPC(posX,posY,0,15,15,WANDER,m_maze);
		AddGameObject2D(npc,500 + i);

		// add to the npc list
		m_npcList.push_back(npc);
	}

}

// stops the ai from sticking to each other
void Game::DeClusterAI()
{
	//// iterate through the npc list
	//list<NPC *>::iterator current = m_npcList.begin();

	//float threshold = 30;

	//for(; current != m_npcList.end(); current++)
	//{
	//	list<NPC *>::iterator other = m_npcList.begin();

	//	for(; other != m_npcList.end(); other++)
	//	{
	//		// make sure were not checking the same npc
	//		if(current != other && (*current) != 0 && (*other) != 0)
	//		{
	//			Vector2 vec = (*current)->Position() - (*other)->Position();
	//			
	//			// if the entities are too close
	//			if(vec.LengthSquared() < threshold * threshold)
	//			{
	//				(*current)->SetVelocityX((*current)->VelocityX() + (vec.X * 0.1));
	//				(*current)->SetVelocityY((*current)->VelocityY() + (vec.Y * 0.1));
	//			}
	//		}
	//	}// end of inner for
	//}// end of outer for


	map<int,GameObject2D*>::iterator current = m_gameObjectMap.begin();

	float pushApart_threshold = 30;
	float squadMerge_threshold = 450;

	// traverse the object map 
	for(;current != m_gameObjectMap.end(); current++)
	{
		int ID = current->first;
		
		// check each npc
		if(ID >500 && ID < 28000 && current->second != 0)
		{
				map<int,GameObject2D*>::iterator other = m_gameObjectMap.begin();

				// traverse the object map
				for(;other != m_gameObjectMap.end(); other++)
				{
					int otherID = other->first;

					// check each entities
					if(otherID > 500 && other->second != 0)
					{
						Vector2 vec = current->second->Position() - other->second->Position();

						float vecLengthSquared = vec.LengthSquared();

						// if the entities are too close
						if(vecLengthSquared < pushApart_threshold * pushApart_threshold)
						{
							current->second->SetVelocityX(current->second->VelocityX() + (vec.X * 0.3));
							current->second->SetVelocityY(current->second->VelocityY() + (vec.Y * 0.3));
						}
						
						// lets see if this is an NPC
						NPC * npc = dynamic_cast<NPC *>(other->second);
						
						// this is actually an NPC and they are chasing a player
						// then we want to draw in the other npc if the distance between them sufficient
						if(npc != 0 && npc->CurrentState() == CHASE)
						{
							// call over other entities within range if we are chasing the enemy
							if(vecLengthSquared < squadMerge_threshold * squadMerge_threshold)
							{
								current->second->SetVelocityX(current->second->VelocityX() - vec.X * 0.01);
								current->second->SetVelocityY(current->second->VelocityY() - vec.Y * 0.01);
							}
							
						}
					}
				}// end of inner for
		}
	}// end of outer for
}

void Game::Update(Vector2 mouse)
{

	// ========== networking ===================================
	// receive all network data from clients
	ReceiveData();

	map<int,GameObject2D*>::iterator current = m_gameObjectMap.begin();
	// traverse the object map and update
	for(;current != m_gameObjectMap.end(); current++)
	{

		if(current->second != 0)
		{
			current->second->Update();
			//m_maze->ResolveAnyCollisions(current->second);
		}
		if(current->second != 0)
		{
			m_maze->ResolveAnyCollisions(current->second);
		}
		
	}

	// check projectile collision with entities
	CheckHits();

	// stop the NPC's from intersecting
	DeClusterAI();

	if(timeGetTime() > m_lastPacketSentTime + SEND_TIME)
	{
		// send data to our clients
		SendData();

		m_lastPacketSentTime = timeGetTime();
	}
	// =========================================================
}

void Game::Draw()
{
	// update our camera view before we draw
	camera->Update();

	glPushMatrix();
		glTranslatef(700,0,0);
		GLwindow::GetInstance()->Print("Packets received: %4.1i",m_packetsReceived);
	glPopMatrix();
	
	// draw the maze
	glPushMatrix();
		m_maze->Draw();
	glPopMatrix();

	// traverse the list of all game objects and draw them
	map<int,GameObject2D*>::iterator current = m_gameObjectMap.begin();

	for(;current!= m_gameObjectMap.end();current++)
	{
		glPushMatrix();
		if(current->second != 0)
		{
			current->second->Draw();
			// print port
			GLwindow::GetInstance()->Print("ID: %4.0i ", current->first);
		}	
		glPopMatrix();
	}
}

void Game::AddGameObject2D(GameObject2D * object, int ID)
{
	m_gameObjectMap[ID] = object;
}

bool Game::RemoveGameObject2D(int ID)
{
	bool objectRemoved = false;

	if(m_gameObjectMap[ID]!=0)
	{
		delete m_gameObjectMap[ID];
		m_gameObjectMap[ID] = 0;

		objectRemoved = true;
	}
	return objectRemoved;
}

// draws a simple text hud
void Game::DrawHud()
{
	GLwindow::GetInstance()->Print("AI_NET: SERVER");

}

// receive data from al clients
void Game::ReceiveData()
{
	// ========== test networking =============================
	
	// data buffer
	char recieved[1000] = "";
	
	// returns true if there is legitimate data
	bool rec = Net::GetInstance()->ReceiveData(recieved);
	
	// while there is legitimate data
	while(rec)
	{
		// the port number of the sender of the last received message
		int portID = Net::GetInstance()->GetLastReceivingPort();

		//===== check if we have received data from this port before ============
		map<int,GameObject2D*>::iterator current = m_gameObjectMap.begin();
		bool portFound = false;
		// run through our map of objects
		for(;current != m_gameObjectMap.end(); current++)
		{
			// if we have received from this port before 
			if(current->first == portID)
			{
				portFound = true;
			}
		}
		// if we never received on this port before and we havent exceeded the max players
		if(!portFound )
		{
			// create a new player on the server
			Player * p = new Player(1200,500,playerTexture,15,15);
			p->SetAmmo(2000);
			AddGameObject2D(p,portID);
		}
		// ======================================================================
			
			// create a bitpacker 
			BitPacker bp;

			// pack in the data from the buffer
			bp.SetData(recieved);
			
			// read the first message
			int value = bp.ReadInt(32);
			
			// while we havent reached the end of the message
			while(value != ENDOFMESSAGE)
			{
				bool dead = false;	
				if(m_gameObjectMap[portID] == 0)
				{
					dead = true;
				}
					//float speed = 10.0f * Timing::GetInstance()->Elapsed();
					float speed = 5;
					
					float velocityX = 0; 
					float velocityY = 0;

					if(!dead)
					{
						velocityX = m_gameObjectMap[portID]->VelocityX();
						velocityY = m_gameObjectMap[portID]->VelocityY();
					}
			
					// input
					if(value == UP)
					{
						if(!dead)
						{
							m_gameObjectMap[portID]->SetVelocityY(velocityY - speed);
						}
					}
					else if(value == DOWN)
					{
						if(!dead)
						{
							m_gameObjectMap[portID]->SetVelocityY(velocityY + speed);
						}
					}
					else if(value == LEFT)
					{
						if(!dead)
						{
							m_gameObjectMap[portID]->SetVelocityX(velocityX - speed);
						}
					}
					else if(value == RIGHT)
					{
						if(!dead)
						{
							m_gameObjectMap[portID]->SetVelocityX(velocityX + speed);
						}
					}
					else if(value == FIRE)
					{
						Vector2 target;
						target.X = bp.ReadFloat(32);
						target.Y = bp.ReadFloat(32);

						if(!dead)
						{
							if(numBullets >= 500)
							{
								numBullets = 0;
							}
							
							// if we have enough ammo then shoot
							if(m_gameObjectMap[portID]->Ammo() > 0)
							{
								m_gameObjectMap[portID]->FireProjectile(target,numBullets);
								numBullets++;
							}
						}
					}

					// read all state data sent
					else if(value == STATE)
					{
						float x = bp.ReadFloat(32);
						float y = bp.ReadFloat(32);

						float angle = bp.ReadFloat(32);
						if(!dead)
						{
							m_gameObjectMap[portID]->SetAngleY(angle);
						}
						
					}

					// read the next value in the packet
 					value = bp.ReadInt(32);
				}// end of while
				
				// increment the number of packets received
				m_packetsReceived++;

				// see if there are anymore packets to read
				rec = Net::GetInstance()->ReceiveData(recieved);
			
	}// end of while

	//===============================================================
}// end of receive data

void Game::SendData()
{
	// create a bitpacker to write to 
	BitPacker bp;
	
	// get the first object in the map 
	map<int,GameObject2D*>::iterator current = m_gameObjectMap.begin();

	// write all the data of all entities to a packet
	for(;current != m_gameObjectMap.end(); current ++)
	{	
		// make sure this entity is a client
		if(current->second != 0 && current->first >= 28000)
		{
			// firstly write the current entities data to the packet ============
			bp.WriteInt(ID,32);
			bp.WriteInt(current->first,32);
							
			// then write the position
			bp.WriteInt(STATE,32);
			bp.WriteFloat(current->second->X(),32);
			bp.WriteFloat(current->second->Y(),32);

			// write the velocity
			bp.WriteFloat(current->second->VelocityX(),32);
			bp.WriteFloat(current->second->VelocityY(),32);
							
			// write the angle
			bp.WriteFloat(current->second->GetAngleY(),32);

			bp.WriteFloat(current->second->Ammo(),32);
			//===================================================================

			// now loop through the other entities
			map<int,GameObject2D*>::iterator other = m_gameObjectMap.begin();

			for(;other != m_gameObjectMap.end(); other++)
			{
				// make sure this object is valid and is not the object we are checking
				if(other->second != 0 && other->second != current->second)
				{
					// do area of interest management
					// only write data to the packet if an entity is in range

					int WorldRight = current->second->Position().X + (GLwindow::GetInstance()->Width()/2);
					int WorldLeft = current->second->Position().X - (GLwindow::GetInstance()->Width()/2);
					int WorldTop = current->second->Position().Y + (GLwindow::GetInstance()->Height()/2);
					int WorldBottom = current->second->Position().Y - (GLwindow::GetInstance()->Height()/2);
					
					// if an entity is in this entities camera view
				    if(other->second->Right() > WorldLeft &&
					  other->second->Left() < WorldRight &&
					  other->second->Top() > WorldBottom &&
					  other->second->Bottom() < WorldTop)
					 {
						// now write the data of the other entities ============
						bp.WriteInt(ID,32);
						bp.WriteInt(other->first,32);
										
						// then write the position
						bp.WriteInt(STATE,32);
						bp.WriteFloat(other->second->X(),32);
						bp.WriteFloat(other->second->Y(),32);

						// write the velocity
						bp.WriteFloat(other->second->VelocityX(),32);
						bp.WriteFloat(other->second->VelocityY(),32);
										
						// write the angle
						bp.WriteFloat(other->second->GetAngleY(),32);

						// write how much ammo we have
						bp.WriteFloat(current->second->Ammo(),32);
						//===================================================================
					}
				}
			}// end of inner for

			// now send the data
			bp.WriteInt(ENDOFMESSAGE,32);
			Net::GetInstance()->SendData(bp.GetData(),"127.0.0.1",current->first);
			bp.Reset();

		}// end of if current is valid

	}// end of outer for
	
}
