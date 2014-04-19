#include "precompiled.h"
#include "GameObject2D.h"
#include "Game.h"


GameObject2D::GameObject2D(float x, float y , int texture , float width , float height )
: m_position(x,y),m_velocity(0,0), m_yAngle(0),m_texture(texture),m_dimensions(width,height),m_friction(0.98f),m_ammo(0),m_health(100)
{
	// add this game object to the list of all game objects
	//Game::AddGameObject2D(this);
}

GameObject2D::~GameObject2D(void)
{
	// remove from the list of all game objects
	//Game::RemoveGameObject2D(this);
}

void GameObject2D::Update()
{
	// update our position by our velocity
	m_position.X += m_velocity.X * Timing::GetInstance()->Elapsed();
	m_position.Y += m_velocity.Y * Timing::GetInstance()->Elapsed();

	// apply friction only if we are moving
	float speed = m_velocity.Length();
	if(speed > 0)
	{
		//float friction = m_friction * Timing::GetInstance()->Elapsed();
		m_velocity =  m_velocity * m_friction;
	}

	if(m_yAngle > 360 || m_yAngle < -360)
	{
		m_yAngle = 0;
	}

}

void GameObject2D::Draw()
{
	glTranslatef(m_position.X,0,m_position.Y);
    glRotatef(m_yAngle,0,1,0);

	// draw our sprite
	glBindTexture(GL_TEXTURE_2D,m_texture);
	glBegin(GL_QUADS);
			 glTexCoord2i(0,0);	glVertex3i(-m_dimensions.X/2 ,1, m_dimensions.Y/2);
             glTexCoord2i(0,1);	glVertex3i(-m_dimensions.X/2,1, -m_dimensions.Y/2);
             glTexCoord2i(1,1);	glVertex3i(m_dimensions.X/2,1, -m_dimensions.Y/2);
             glTexCoord2i(1,0);	glVertex3i(m_dimensions.X/2,1, m_dimensions.Y/2);
     glEnd();

#if _DEBUG
	//draw a bounding box around the object if in debug
	 glColor3f(1.0f,0.0f,0.0f);
	 glBegin(GL_LINES);
	    // top line
		glVertex3f(-m_dimensions.X/2,0,-m_dimensions.Y/2);
		glVertex3f(m_dimensions.X/2,0,-m_dimensions.Y/2);

		// left line
		glVertex3f(-m_dimensions.X/2,0,-m_dimensions.Y/2);
		glVertex3f(-m_dimensions.X/2,0,m_dimensions.Y/2);

		//bottom line
		glVertex3f(-m_dimensions.X/2,0,m_dimensions.Y/2);
		glVertex3f(m_dimensions.X/2,0,m_dimensions.Y/2);

		// right line
		glVertex3f(m_dimensions.X/2,0,-m_dimensions.Y/2);
		glVertex3f(m_dimensions.X/2,0,m_dimensions.Y/2);
	 glEnd();
	 glColor3f(1.0f,1.0f,1.0f);

#endif
}

void GameObject2D::FireProjectile(Vector2 position,int id)
{
	Vector2 between = position - m_position;

	between.Normalise();

	Projectile * p = new Projectile(id,m_position.X + (between.X * m_dimensions.X),m_position.Y + (between.Y * m_dimensions.Y),0,1,1,1000);

	float speed = 400;

	p->SetVelocityX(between.X * speed);
	p->SetVelocityY(between.Y * speed);

	Game::AddGameObject2D(p,id);

	m_ammo--;
}

// rotate the direction vector around the y axis
//void GameObject2D::RotateY(float angle)
//{
//	m_yAngle+=angle;
//    angle=(angle*PI)/180;//converts to radians
//	
//	Vector2 temp;
//	temp.X = m_direction.Y*sin(angle)+m_direction.X*cos(angle);
//	temp.Y = m_direction.Y*cos(angle)-m_direction.X*sin(angle);
//
//	m_direction = temp;
//
//}
