#include "precompiled.h"
#include "Player.h"

Player::Player(float x, float y, int texture, int width, int height): GameObject2D(x,y,texture,width,width)
{
}
Player::~Player(void)
{
}
void Player::Update()
{
	GameObject2D::Update();
}

