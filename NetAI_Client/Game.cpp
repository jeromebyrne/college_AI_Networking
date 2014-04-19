#include "precompiled.h"
#include "Game.h"
#include "BitPacker.h"
#define STATE_TIMEOUT 500 // dont draw entities which exceed this time in which we have received data

// initialise the static list
map<int,GameObject2D *> Game::m_gameObjectMap;

Vector2 Game::m_mousePos;

Game::Game(void): m_lastSendPacketTime(0),playerTexture(0),npcTexture(0),m_numBullets(0)
{
	camera = new BasicCamera();
}

Game::~Game(void)
{
	// travers the list of all game objects and delete them
	//TODO: delete

	if(m_maze != 0)
	{
		delete m_maze;
		m_maze = 0;
	}
	
}

void Game::Initialise()
{
	// initialise our texture manager
	Texture::GetInstance()->Initialise(GLwindow::GetInstance()->GetHwndRef());
	
	// texture for the player sprite
	playerTexture = Texture::GetInstance()->generateTexture("textures/player.bmp");
	
	// texture for enemies
	npcTexture = Texture::GetInstance()->generateTexture("textures/NPC.bmp");
	
	npcTexture2 = Texture::GetInstance()->generateTexture("textures/NPC2.bmp");
	
	// create a new player
	m_player = new Player(1200,500,playerTexture,15,15);
	
	// create a new maze
	m_maze = new Maze(0,0,60);
	m_maze->Build();
	
	// create a new area of interest manager and set it area equal to the maze dimensions
	m_aoiManager = new AOImanager(0,0,m_maze->Width(),m_maze->Height());

	// create a new network connection
	Net::GetInstance()->SetUpConnection(28000);

	// add this client to the game object map
	AddGameObject2D(m_player,Net::GetInstance()->Port());
}

void Game::Update(Vector2 mouse)
{
	m_mousePos = mouse;
	
	//=========== networking =======================
	ReceiveData();
	
	// only do collision detection for this client
	m_maze->ResolveAnyCollisions(m_player);

	m_player->ProcessInput();

	map<int,GameObject2D*>::iterator current = m_gameObjectMap.begin();
	
	// traverse the object map and update
	for(;current != m_gameObjectMap.end(); current++)
	{
		// dont update other clients because we want to interpolate their position
		if(current->second != 0)
		{
			if(current->first <28000)
			{
				current->second->Update();
				//m_maze->ResolveAnyCollisions(current->second);
			}
			else if(current->first != Net::GetInstance()->Port())
			{
				// interpolate other clients
				current->second->Interpolate(0.0001);
			}
			
		}
		
	}
	m_player->Update();

	// check if its time to send a packet
	if(timeGetTime() > m_lastSendPacketTime + PACKET_SEND_TIME)
	{
		m_player->SendData();
		m_lastSendPacketTime = timeGetTime();
	}

	// =========================================================

	// make the camera follow the player
	camera->X = -m_player->X();
	camera->Y = m_player->Y();

}

void Game::Draw()
{
	// update our camera view before we draw
	camera->Update();
	
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
			// only draw entites which we have recently received state for
			if(current->second->LastReceivedStateTime() > GetTickCount() - STATE_TIMEOUT)
			{
				current->second->Draw();
			}
			
		}
			
		glPopMatrix();
	}
}

void Game::AddGameObject2D(GameObject2D * object,int ID)
{
	// push the pointer to the object onto the list
	m_gameObjectMap[ID] = object;
}

bool Game::RemoveGameObject2D(int ID)
{
	bool objectRemoved = false;

	//m_objectList.remove(object);
	if(m_gameObjectMap[ID]!=0)
	{
		delete m_gameObjectMap[ID];
		m_gameObjectMap[ID] = 0;

		objectRemoved = true;

//		m_numPlayersJoined--;

	}

	return objectRemoved;
}

// draws a simple text hud
void Game::DrawHud()
{
	//GLwindow::GetInstance()->Print("AI_NET: CLIENT");
	
	GLwindow::GetInstance()->Print("Ammo: %4i",m_player->Ammo());

}

void Game::ReceiveData()
{
	char received[1000] = "";

	bool rec = Net::GetInstance()->ReceiveData(received);
	
	// if we have received data
	while(rec)
	{
		BitPacker bp;
		bp.SetData(received);

		int value = bp.ReadInt(32);

		int id = 0;
		
		// loop through the packet
		while(value != ENDOFMESSAGE && value != BLANK)
		{
			// if we are reading an ID
			if(value == ID)
			{
				id = bp.ReadInt(32);
				
				bool found = false;

				// iterate through our object map and check if this ID is in the map
				map<int,GameObject2D*>::iterator current = m_gameObjectMap.begin();

				for(;current != m_gameObjectMap.end(); current++)
				{
					if(current->first == id && current->second != 0)
					{
						found = true;
					}
				}

				// if we have never encountered this id (or else we have but it has been removed)
				if(!found)
				{
					// players ID's are port numbers starting from 28000
					if(id >=28000)
					{
						Player * p = new Player(1200,500,playerTexture,15,15);
						AddGameObject2D(p,id);
					}
					// 500 to 28000 is reserved for non playable characters
					else if(id > 500 && id < 28000)
					{
						int randTexture = rand()%2;
						if(randTexture == 0)
						{
							randTexture = npcTexture;
						}
						else
						{
							randTexture = npcTexture2;
						}
						
						GameObject2D * g = new GameObject2D(1200,500,randTexture,15,15);
						AddGameObject2D(g,id);
					}
					else if(id >= 0 && id <= 500)
					{
						Projectile * p = new Projectile(id,0,0,0,10,10,1000,PROJ_NPC);
						AddGameObject2D(p,id);
					}
				}
			}
			// time to read position data
			else if(value == STATE)
			{
				Vector2 pos;
				pos.X = bp.ReadFloat(32); 
				pos.Y = bp.ReadFloat(32);

				Vector2 vel;
				vel.X = bp.ReadFloat(32);
				vel.Y = bp.ReadFloat(32);

				float angle = bp.ReadFloat(32);

				int ammo = bp.ReadInt(32);
				
				// get the relevant object
				GameObject2D * g = m_gameObjectMap[id];

				g->SetAmmo(ammo);

				// if we are updating our own client
				if(id == Net::GetInstance()->Port())
				{
					if(pos.Distance(g->Position()) > 0)
					{
						g->SetX(pos.X);
						g->SetY(pos.Y);
					}

					g->SetVelocityX(vel.X);
					g->SetVelocityY(vel.Y);
				}
				else // this is another client or object
				{
					// interpolate clients (over 28000)
					if(id >= 28000)
					{
						g->m_lerpPosition = pos;

						g->SetAngleY(angle);

						if(pos.Distance(g->Position()) > 60)
						{
							g->SetX(pos.X);
							g->SetY(pos.Y);
						}
					}
					// just update other entities
					else
					{
						if(pos.Distance(g->Position()) > 0)
						{
							g->SetX(pos.X);
							g->SetY(pos.Y);
						}

						g->SetVelocityX(vel.X);
						g->SetVelocityY(vel.Y);

						g->SetAngleY(angle);
					}
				}

				// we just received state data so timestamp
				m_gameObjectMap[id]->SetLastReceivedStateTime();
			}
			value = bp.ReadInt(32);
		} // end of while
		rec = Net::GetInstance()->ReceiveData(received);
	}// end of while received

}
