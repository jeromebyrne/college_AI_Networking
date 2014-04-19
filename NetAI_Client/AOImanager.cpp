#include "precompiled.h"
#include "AOImanager.h"

AOImanager::AOImanager(float x, float y, float width, float height):m_position(x,y),m_worldDimensions(width, height)
{
}

AOImanager::~AOImanager(void)
{
}

void AOImanager::DrawGrid()
{
	//glTranslatef(m_position.X,0,m_position.Y);
	glColor3f(1.0f,0.0f,0.0f);

	glBegin(GL_LINES);

	// draw all the horizontal splits
	float yGap = m_worldDimensions.Y / HORIZONTAL_SPLITS;
	float currentYpos = m_position.Y;
	for(int i = 1; i <= HORIZONTAL_SPLITS + 1; i++)
	{
		glVertex3f(m_position.X,0,currentYpos);
		glVertex3f(m_position.X + m_worldDimensions.X,0,currentYpos );
		currentYpos += yGap;
	}
	
	// draw all the vertical splits
	float xGap = m_worldDimensions.X / VERTICAL_SPLITS;
	float currentXpos = m_position.X;
	for(int i = 1; i <= VERTICAL_SPLITS + 1; i++)
	{
		glVertex3f(currentXpos,0,m_position.Y);
		glVertex3f(currentXpos,0,m_position.Y + m_worldDimensions.Y );
		currentXpos += xGap;
	}

	glEnd();

	glColor3f(1.0f,1.0f,1.0f);
}

// returns the correct tile index' to search for, returns them in a vector
Vector2 AOImanager::GetTileRegion(Vector2 position, Maze * maze)
{
	Vector2 tileIndex(0,0);
	
	float currentX = 0;
	float xGap = m_worldDimensions.X / VERTICAL_SPLITS;

	for(int i = 0; i < VERTICAL_SPLITS; i++)
	{
		if( position.X >= currentX && position.X <= currentX+xGap)
		{
			//tileIndex.X = 
			//break;
		}

		currentX += xGap;
	}

	return tileIndex;
}
