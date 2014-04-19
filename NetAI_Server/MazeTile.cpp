#include "precompiled.h"
#include "MazeTile.h"

MazeTile::MazeTile(float x, float y , int texture, int width, int type)
: m_position(x,y), m_texture(texture),m_width(width),m_type(type), m_centrePos(x + width/2, y + width/2)
{
}

MazeTile::~MazeTile(void)
{
}

void MazeTile::Draw()
{
	//glBindTexture(GL_TEXTURE_2D,m_texture);
	
	glPushMatrix();
		glBegin(GL_QUADS);

		/*glTexCoord2i(0,0);*/ glVertex3i(m_position.X ,-1, m_position.Y);
		/*glTexCoord2i(0,1);*/ glVertex3i(m_position.X,-1, m_position.Y + m_width);
		/*glTexCoord2i(1,1);*/ glVertex3i(m_position.X + m_width,-1, m_position.Y + m_width);
		/*glTexCoord2i(1,0);*/ glVertex3i(m_position.X + m_width,-1, m_position.Y);

		glEnd();

	 glPopMatrix();
}

