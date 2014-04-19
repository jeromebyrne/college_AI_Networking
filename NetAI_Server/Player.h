#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject2d.h"

class Player : public GameObject2D
{
private:
public:
	Player(float x = 0, float y = 0, int texture = 0, int width = 5, int height = 5);
	~Player(void);
	void Update();

};

#endif
