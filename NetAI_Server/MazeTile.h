#ifndef MAZETILE_H
#define MAZETILE_H

class MazeTile
{
private:
	// the texture for this tile
	int m_texture;
	
	// the type this tile is
	int m_type;

public:
	Vector2 m_position;
	Vector2 m_centrePos;
	int m_width;

	MazeTile(float x = 0, float y = 0, int texture = 0, int width = 3, int type = FLOOR);
	~MazeTile(void);
	void Draw();

	inline int GetType()
	{
		return m_type;
	}
};

#endif
