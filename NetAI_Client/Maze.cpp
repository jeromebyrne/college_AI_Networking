#include "precompiled.h"
#include "Maze.h"

Maze::Maze(float x, float y, float tilewidth): m_position(x,y),m_tileWidth(tilewidth)
{
}

Maze::~Maze(void)
{
}
void Maze::Build()
{
	int tileTypes[ROWS][COLUMNS] = 
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	 3,2,0,0,0,2,0,0,3,0,0,0,3,0,0,3,3,2,0,0,0,3,0,3,0,0,0,3,2,0,2,3,0,0,2,0,0,0,0,3,
	 3,0,3,3,3,3,0,0,3,0,3,0,3,0,3,3,0,0,0,3,3,0,0,2,0,3,0,3,0,3,0,3,0,3,3,3,3,3,0,3,
	 3,0,3,0,0,0,0,0,3,0,3,2,0,0,3,3,3,2,0,3,3,3,0,3,3,3,0,0,0,3,0,3,0,3,2,3,0,3,0,3,
	 3,0,3,0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,2,0,0,0,0,3,0,0,0,0,0,3,0,3,
	 3,0,3,3,3,3,3,0,3,0,3,3,0,3,3,3,0,0,0,0,0,0,0,0,3,3,3,0,3,3,0,3,0,3,0,3,3,3,0,3,
	 3,0,0,0,0,2,0,0,0,0,0,3,0,3,2,0,0,3,3,0,0,3,3,0,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,3,
	 3,3,3,3,3,3,3,0,3,0,0,3,0,3,0,0,0,3,0,0,0,0,3,0,0,0,3,0,3,0,0,3,0,3,3,3,3,3,3,3,
	 3,0,0,0,0,0,3,0,3,0,0,0,2,0,0,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0,0,3,0,3,0,0,0,0,0,3,
	 3,0,3,3,3,0,3,0,3,0,3,3,3,3,3,0,0,3,3,3,3,3,3,0,0,3,3,3,3,3,0,3,0,3,2,3,3,3,0,3,
	 3,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,
	 3,3,3,3,3,3,3,0,3,0,0,0,0,0,3,3,3,3,0,3,3,0,3,3,3,3,0,0,0,0,0,3,0,3,3,3,3,3,3,3,
	 3,0,0,0,2,0,0,0,0,0,0,0,3,2,3,0,0,0,0,3,3,0,0,2,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,3,
	 3,0,3,3,3,0,3,0,3,0,3,3,3,0,3,0,2,0,0,0,0,0,0,0,2,3,0,3,3,3,0,3,0,3,3,3,3,3,0,3,
	 3,0,3,0,0,0,2,0,3,0,0,0,3,0,3,3,3,3,0,3,3,0,3,3,3,3,0,3,0,0,0,3,0,3,0,0,2,3,0,3,
	 3,0,3,0,3,0,3,0,3,0,3,2,0,2,0,2,0,0,0,3,3,0,0,0,0,0,0,0,0,3,0,3,0,3,0,3,0,3,0,3,
	 3,2,3,0,0,2,3,0,3,0,3,0,3,0,3,3,3,3,3,3,3,3,3,3,3,3,0,3,0,3,0,3,0,0,0,0,0,3,0,3,
	 3,0,3,3,3,3,3,0,3,0,3,0,3,2,3,0,0,0,3,0,0,0,3,0,0,0,0,3,0,3,0,3,0,0,0,3,3,3,0,3,
	 3,0,0,0,0,0,0,0,3,0,0,0,3,0,0,0,3,2,0,0,3,0,0,0,3,0,0,3,0,0,0,3,0,0,0,0,0,0,2,3,
	 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};

	int floorTexture = Texture::GetInstance()->generateTexture("textures/grass.bmp");
	int wallTexture = Texture::GetInstance()->generateTexture("textures/stone.bmp");
	int sandPatchTexture = Texture::GetInstance()->generateTexture("textures/sandPatch.bmp");
	int slabTexture = Texture::GetInstance()->generateTexture("textures/slab.bmp");

	// the position of each tile 
	float tile_x = m_position.X, tile_y = m_position.Y;

	// create all of our track tiles using the types stored in the array
	for(int i = 0; i < ROWS; i++)
	{
		// set the tile x position back to the top left corner (track position)
		tile_x = m_position.X;

		for(int count = 0; count < COLUMNS; count++)
		{
			if(tileTypes[i][count] == GRASS)
			{
				MazeTile tile(m_position.X + tile_x, m_position.Y + tile_y, floorTexture, m_tileWidth,GRASS);
				m_tiles[i][count] = tile;
			}
			else if(tileTypes[i][count] == WALL)
			{
				MazeTile tile(m_position.X + tile_x, m_position.Y + tile_y, wallTexture, m_tileWidth,WALL);
				m_tiles[i][count] = tile;
			}
			else if(tileTypes[i][count] == SAND)
			{
				MazeTile tile(m_position.X + tile_x, m_position.Y + tile_y, sandPatchTexture, m_tileWidth,GRASS);
				m_tiles[i][count] = tile;
			}
			else if(tileTypes[i][count] == SLAB)
			{
				MazeTile tile(m_position.X + tile_x, m_position.Y + tile_y, slabTexture, m_tileWidth,WALL);
				m_tiles[i][count] = tile;
			}
			// increment the tile x position to the next tile
			tile_x += m_tileWidth;
		}// end of inner for

		// increment the tile position y position to the next tile
		tile_y += m_tileWidth;
		
	}// end of outer for

	
}

void Maze::Draw()
{
	//glTranslatef(m_position.X,0,m_position.Y);
	for(int i =0; i<ROWS; i++)
	{
		for(int count = 0; count < COLUMNS; count++)
		{
			//if(m_tiles[i][count].GetType() == WALL)
			//{
				m_tiles[i][count].Draw();
			//}
		}
	}
}

bool Maze::ResolveAnyCollisions(GameObject2D * g)
{
	bool collisionWithSolid = false;

	// search the entire map
	// TODO: use area of interest manager to find appropriate tiles
	for(int i = 0; i < ROWS; i++)
	{
		for(int count = 0; count < COLUMNS; count++)
		{
			// check if it is solid (wall), only do a collision check if solid
			if(m_tiles[i][count].GetType() == WALL)
			{
				// find the bounds of the tile
				float wallLeft = m_tiles[i][count].m_position.X ;
				float wallRight = m_tiles[i][count].m_position.X + m_tileWidth ;
				float wallTop = m_tiles[i][count].m_position.Y + m_tileWidth ;
				float wallBottom = m_tiles[i][count].m_position.Y ;

				// if the x and y are within the bounds of this tile
				if((g->Right() > wallLeft				  &&
				   g->Left() < wallRight				  &&
				   g->Top() > wallBottom				  &&
				   g->Bottom() < wallTop))
				{
					// we hit a solid tile
					collisionWithSolid = true;

					// now resolve the collision
					float xOverlap;
					float yOverlap;

					// if the gameObject lies left of the centre of the wall
					if (g->X() < wallLeft + m_tileWidth/2)
					 {
						 xOverlap = wallLeft  - g->Right();
					 }
					 else // it lies right of the centre
					 {
						 xOverlap = wallRight - g->Left();
					 }

					 // if the point is above the center of the wall
					if (g->Y() < wallBottom + m_tileWidth/2)
					 {
						 yOverlap = wallBottom - g->Top();
					 }
					 else// it lies below the centre of the wall
					 {
						 yOverlap = wallTop - g->Bottom();
					 }

					 float tempXoverlap = xOverlap;
					 float tempYoverlap = yOverlap;

					 // if the overlap is negative then change to positive by multiplying by -1
					 // this is only done to compare the size of the xOverlap and yOverlap,
					 // these new values are stored in temporary variables and the originals are
					 // not touched
					 if (tempXoverlap < 0)
					 {
						 tempXoverlap = -tempXoverlap ;
					 }
					 if (tempYoverlap < 0)
					 {
						 tempYoverlap = -tempYoverlap;
					 }
					 //======================================================

					 // now determine which overlap is greater,
					 // we are going to move the point along the axis which
					 // has the shortest overlap.
						
					 float damping = 0.5f;
					 if (tempXoverlap <= tempYoverlap)
					 {
						 g->SetX(g->X() + xOverlap );

						 // reverse the velocity and dampen it
						 g->SetVelocityX(g->VelocityX() * -damping);
					 }
					 else
					 {
						 g->SetY(g->Y() + yOverlap);

						 // reverse the velocity and dampen it
	                     g->SetVelocityY(g->VelocityY() * -damping);
					 }

						break;
					}
			}
			
		}// end of inner for
	}// end of outer for

	return collisionWithSolid;
}
