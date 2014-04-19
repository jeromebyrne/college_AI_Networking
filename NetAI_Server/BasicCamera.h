#ifndef BASICCAMERA_H
#define BASICCAMERA_H

class BasicCamera
{
	
	public:
	float X;
	float Y;
	float Z;

	float ForwardX;
	float ForwardY;
	float ForwardZ;

	float xAngle;

	BasicCamera(void)
	{
		X = -1200;
		Y = 600;
		Z = -2000;
		ForwardX = 0;
		ForwardY = 1;
		ForwardZ = 0;

		xAngle = 90;
	}

	~BasicCamera(void)
	{
	}

	void Update()
	{
		glTranslatef(X,Y,Z);
		glRotatef(xAngle,1,0,0);

		

		// we can zoom in and out if in debug
		if(GetAsyncKeyState('I')<0)
		{
			Z+= 12;
		}
		else if(GetAsyncKeyState('O')<0)
		{
			Z-= 12;
		}

		if(GetAsyncKeyState(VK_LEFT) < 0)
		{
			X += 12;
		}
		else if(GetAsyncKeyState(VK_RIGHT) < 0)
		{
			X -= 12;
		}

		if(GetAsyncKeyState(VK_UP) < 0)
		{
			Y -= 12;
		}
		else if(GetAsyncKeyState(VK_DOWN) < 0)
		{
			Y += 12;
		}


	}

	void MoveForward(float speed)
	{
		X += ForwardX * speed;
		Y += ForwardY * speed;
		Z -= ForwardZ * speed;
	}
	void MoveBackward(float speed)
	{
		X -= ForwardX * speed;
		Y -= ForwardY * speed;
		Z += ForwardZ * speed;
	}
	void MoveLeft(float speed)
	{
		X -= speed;
	}
	void MoveRight(float speed)
	{
		X += speed;
	}
};
#endif
