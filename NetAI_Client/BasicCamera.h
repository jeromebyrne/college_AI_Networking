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
		X = 0;
		Y = 0;
		Z = -381;
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

		
#if _DEBUG
		//// we can zoom in and out if in debug
		//if(GetAsyncKeyState(VK_UP)<0)
		//{
		//	Z+= 8;
		//}
		//else if(GetAsyncKeyState(VK_DOWN)<0)
		//{
		//	Z-= 8;
		//}

		//if(GetAsyncKeyState(VK_LEFT) < 0)
		//{
		//	xAngle+=0.5f;
		//}
		//else if(GetAsyncKeyState(VK_RIGHT) < 0)
		//{
		//	xAngle-=0.5f;
		//}
#endif

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
