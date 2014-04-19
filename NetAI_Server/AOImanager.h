#ifndef AOIMANAGER_H
#define AOIMANAGER_H

#define HORIZONTAL_SPLITS 4
#define VERTICAL_SPLITS 8

#include "Maze.h"

class AOImanager
{
private:

	Vector2 m_position;

	// needs to know the world width and height
	Vector2 m_worldDimensions;
public:
	 AOImanager(float x = 0, float y = 0, float width = 10, float height = 10);
	~AOImanager(void);

	inline void SetWorldWidth(float value)
	{
		m_worldDimensions.X = value;
	}
	inline void SetWorldHeight(float value)
	{
		m_worldDimensions.Y = value;
	}
	inline float X()
	{
		return m_position.X;
	}
	inline float Y()
	{
		return m_position.Y;
	}
	inline void SetX(float value)
	{
		m_position.X = value;
	}
	inline void SetY(float value)
	{
		m_position.Y = value;
	}
	
	// draw the regions using a grid of lines
	void DrawGrid();

	// returns the correct tile index' to search for, returns them in a vector
	Vector2 GetTileRegion(Vector2 position,Maze * maze);
};

#endif
