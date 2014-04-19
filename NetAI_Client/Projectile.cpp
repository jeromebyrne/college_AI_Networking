#include "precompiled.h"
#include "Projectile.h"
#include "Game.h"

Projectile::Projectile(int id, float x, float y, int texture, float width, float height, int lifeTime, int owner)
:GameObject2D(x,y,texture,width,height),m_lifeTime(lifeTime),m_id(id),m_owner(owner)
{
	this->SetFriction(1.0f);
	m_birthTime = timeGetTime();
}

Projectile::~Projectile(void)
{
}
void Projectile::Draw()
{
	
	if(m_owner == PROJ_PLAYER)
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	else
	{
		glColor3f(1.0f,0.0f,0.0f);
	}
	
	glLineWidth(4.0f);

	glBegin(GL_LINES);
		glVertex3f(X()-1,0,Y()-1);
		glVertex3f(X()+1,0,Y()+1);

		glVertex3f(X()+1,0,Y()-1);
		glVertex3f(X()-1,0,Y()+1);
	glEnd();

	glLineWidth(1.0f);
	glColor3f(1.0f,1.0f,1.0f);

}

void Projectile::Update()
{
	GameObject2D::Update();
	
	// if we exceed our life time then remove from level
	if(timeGetTime() > m_birthTime+m_lifeTime)
	{
		Game::RemoveGameObject2D(m_id);
	}
}
