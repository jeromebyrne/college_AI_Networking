#ifndef MAZE_H
#define MAZE_H

#define ROWS 20
#define COLUMNS 40

#include "MazeTile.h"
#include "GameObject2D.h"

class Maze
{
private:
	// the position of our maze
	Vector2 m_position;

	// the width of the tiles that make up the maze
	int m_tileWidth;

	// an array of all the track tiles
	MazeTile m_tiles[ROWS][COLUMNS];

	// the differen types of tiles in the maze
	enum ENUM_tileTypes
	{
		FLOOR,
		WALL,
		NODE
	};

public:
	Maze(float x = 0, float y = 0, float tilewidth = 2);
	~Maze(void);
	void Build();
	void Draw();

	// the width of the map
	inline int Width()
	{
		return m_tileWidth * COLUMNS;
	}
	// the height of the map
	inline int Height()
	{
		return m_tileWidth * ROWS;
	}
	
	// check if a game object collides with a wall and resolve, return true if there was a collision 
	bool ResolveAnyCollisions(GameObject2D * gameObject);

	// takes an xy position and finds this cell and then returns the adjacent cells
	list<MazeTile>  GetAdjacentTiles(float x, float y);

	// return the array of tiles
	inline MazeTile * GetTiles()
	{
		return m_tiles[0];
	}
	
};

#endif
