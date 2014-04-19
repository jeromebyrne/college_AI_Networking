#include "precompiled.h"
#include "GameObject2D.h"
#include "Game.h"


GameObject2D::GameObject2D(float x, float y , int texture , float width , float height )
: m_position(x,y),m_velocity(0,0), m_yAngle(90),m_texture(texture),m_dimensions(width,height),m_friction(0.98),m_oldPosition(x,y),
m_lerpPosition(0,0),m_interpolationValue(0),m_ammo(0),m_lastStateReceivedTime(0)
{

}

GameObject2D::~GameObject2D(void)
{

}

void GameObject2D::Interpolate(float lerpValue)
{
	float blah = (1 - m_interpolationValue);
	Vector2 value1 = m_position * blah;
	Vector2 value2 = m_lerpPosition * m_interpolationValue;

	Vector2 newPos =  value1 + value2;

	SetX(newPos.X);
	SetY(newPos.Y);

	m_interpolationValue += lerpValue;

	if(m_interpolationValue > 1)
	{
		m_interpolationValue = 0;
	}
}

void GameObject2D::Update()
{
	m_oldPosition = m_position;

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
	Projectile * p = new Projectile(id,m_position.X,m_position.Y,0,10,10,1000);

	Vector2 between = position - m_position;

	between.Normalise();

	float speed = 400;

	p->SetVelocityX(between.X * speed);
	p->SetVelocityY(between.Y * speed);

	Game::AddGameObject2D(p,id);
}

