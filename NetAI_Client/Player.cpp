#include "precompiled.h"
#include "Player.h"
#include "BitPacker.h"
#include "Game.h"


Player::Player(float x, float y, int texture, int width, int height): GameObject2D(x,y,texture,width,width)
{
}
Player::~Player(void)
{
}
void Player::Update()
{
	// update our base object last
	GameObject2D::Update();

	// make the sprite spin... cool!!
	float xSpeed = VelocityX();
	float ySpeed = VelocityY();

	if(xSpeed < 0)
	{
		xSpeed = -xSpeed;
	}
	if(ySpeed < 0)
	{
		ySpeed = -ySpeed;
	}

	if(ySpeed <= xSpeed)
	{
		float value = Timing::GetInstance()->Elapsed() * 2;
		SetAngleY(GetAngleY() + VelocityY() * value);

	}
	else if(xSpeed < ySpeed)
	{
		float value = Timing::GetInstance()->Elapsed() * 2;
		SetAngleY(GetAngleY() - VelocityX() * value);
	}

}
void Player::Draw()
{
	// draw the base object
	GameObject2D::Draw();
	
}

// send all relevant network data to server
void Player::SendData()
{	
	// write our position to the packet
	m_packet.WriteInt(STATE,32);
	m_packet.WriteFloat(m_position.X,32);
	m_packet.WriteFloat(m_position.Y,32);
	m_packet.WriteFloat(m_yAngle,32);
	m_packet.WriteInt(ENDOFMESSAGE,32);
	// send out networking packet to server port
	Net::GetInstance()->SendData(m_packet.GetData(),"127.0.0.1",25000);
	m_packet.Reset();
}

void Player::ProcessInput()
{
	//float speed = 10.0f * Timing::GetInstance()->Elapsed();
	float speed = 5;
	
	// only take input if this is the active window
	if(GetForegroundWindow() == GLwindow::GetInstance()->GetHwndRef())
	{
			// we are just moving up
			if(GetAsyncKeyState('W') < 0)
			{
				SetVelocityY(VelocityY() - speed );

				m_packet.WriteInt(UP,32);
			}

			// we are just moving down
			if(GetAsyncKeyState('S') < 0)
			{
				SetVelocityY(VelocityY() + speed );

				m_packet.WriteInt(DOWN,32);
			}
			// we are just moving left
			if(GetAsyncKeyState('A') < 0)
			{
				SetVelocityX(VelocityX() - speed);

				m_packet.WriteInt(LEFT,32);
			}
			// we are just moving right
			if(GetAsyncKeyState('D') < 0)
			{
				SetVelocityX(VelocityX() + speed);

				m_packet.WriteInt(RIGHT,32);
			}

			// Fire bullet
			if(GetAsyncKeyState(VK_LBUTTON) < 0)
			{
				Vector2 mouse = Game::m_mousePos;

				float mouseWorldPosX = 0;
				float mouseWorldPosY = 0;
				
				int windowWidth = GLwindow::GetInstance()->Width();
				int windowHeight = GLwindow::GetInstance()->Height();


				if(mouse.X >= windowWidth/2)
				{
					mouseWorldPosX = m_position.X + (mouse.X - windowWidth/2);
				}
				else
				{
					mouseWorldPosX = m_position.X - (windowWidth/2 - mouse.X);
				}
				if(mouse.Y <= windowHeight/2)
				{
					mouseWorldPosY = m_position.Y - (windowHeight/2 - mouse.Y);
				}
				else
				{
					mouseWorldPosY = m_position.Y + (mouse.Y - windowHeight/2);
				}

				Vector2 p(mouseWorldPosX,mouseWorldPosY);

				int TIME_CAN_FIRE = 100; // we can fire every N milliseconds
				static int lastTimeFired = 0;

				if(GetTickCount() - lastTimeFired  > TIME_CAN_FIRE)
				{
					
					// here we make a request to fire
					m_packet.WriteInt(FIRE,32);
					
					// write the position we want to fire towards
					m_packet.WriteFloat(p.X,32);
					m_packet.WriteFloat(p.Y,32);

					lastTimeFired = GetTickCount();
				}
					
			}


	}// end of input
}
