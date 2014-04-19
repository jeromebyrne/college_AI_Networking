#include "precompiled.h"
#include "GameObject3D.h"

GameObject3D::GameObject3D(void)
{
}

GameObject3D::GameObject3D(float x = 0, float y = 0, float z = 0)
: m_position(x,y,z), m_yAngle(0)
{
}

GameObject3D::~GameObject3D(void)
{
}

void GameObject3D::Update()
{
}

void GameObject3D::Draw()
{
}