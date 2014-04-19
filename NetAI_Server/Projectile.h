#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameobject2d.h"

class Projectile :
	public GameObject2D
{
private:
	int m_id;
	int m_lifeTime;
	int m_birthTime;
public:
	Projectile(int id = 0,float x = 0, float y = 0, int texture = 0, float width = 1, float height = 1, int m_lifeTime = 3000);
	virtual ~Projectile(void);
	void Draw();
	void Update();
};

#endif
