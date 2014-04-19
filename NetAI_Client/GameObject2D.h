#ifndef GAMEOBJECT2D_H
#define GAMEOBJECT2D_H

class GameObject2D
{
private:

protected:
	Vector2 m_position;
	Vector2 m_oldPosition;
	Vector2 m_velocity;
	Vector2 m_dimensions;

	// how much the entity is rotated around the y axis
	float m_yAngle;
	int m_texture;

	float m_friction;

	int m_ammo;

	// the last time we received state data
	int m_lastStateReceivedTime;
	
public:
	GameObject2D(float x = 0, float y = 0, int texture = 0, float width = 1, float height = 1);
	virtual ~GameObject2D(void);

	virtual void Update();
	virtual void Draw();

	float m_interpolationValue;

	Vector2 m_lerpPosition;

	void Interpolate(float lerpValue);
	
	// returns the last time we received state data
	inline int LastReceivedStateTime()
	{
		return m_lastStateReceivedTime;
	}

	// sets the last time we received state data to now
	inline void SetLastReceivedStateTime()
	{
		m_lastStateReceivedTime = GetTickCount();
	}

	inline int Ammo()
	{
		return m_ammo;
	}

	inline void SetAmmo(int value)
	{
		m_ammo = value;
	}

	inline Vector2 Position()
	{
		return m_position;
	}

	inline Vector2 OldPosition()
	{
		return m_oldPosition;
	}

	inline void SetOldPosition(float x, float y)
	{
		m_oldPosition.X = x;
		m_oldPosition.Y = y;
	}

	inline float X()
	{
		return m_position.X;
	}
	inline float Y()
	{
		return m_position.Y;
	}
	inline void SetX(float value)
	{
		m_position.X = value;
	}
	inline void SetY(float value)
	{
		m_position.Y = value;
	}
	inline Vector2 Velocity()
	{
		return m_velocity;
	}
	inline float VelocityX()
	{
		return m_velocity.X;
	}
	inline float VelocityY()
	{
		return m_velocity.Y;
	}
	inline void SetVelocityX(float value)
	{
		m_velocity.X = value;
	}
	inline void SetVelocityY(float value)
	{
		m_velocity.Y = value;
	}
	inline void SetFriction(float value)
	{
		m_friction = value;
	}
	inline float GetAngleY()
	{
		return m_yAngle;
	}
	inline void SetAngleY(float value)
	{
		m_yAngle = value;
	}
	inline float Left()
	{
		return m_position.X - m_dimensions.X/2;
	}
	inline float Right()
	{
		return m_position.X + m_dimensions.X/2;
	}
	inline float Top()
	{
		return m_position.Y + m_dimensions.Y/2;
	}
	inline float Bottom()
	{
		return m_position.Y - m_dimensions.Y/2;
	}
	inline float Width()
	{
		return m_dimensions.X;
	}
	inline float Height()
	{
		return m_dimensions.Y;
	}

	void FireProjectile(Vector2 position,int id);

	// rotate around the y axis
	//void RotateY(float angle); 
	
};

#endif