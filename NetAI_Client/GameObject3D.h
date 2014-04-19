#ifndef GAMEOBJECT3D_H
#define GAMEOBJECT3D_H

class GameObject3D
{
private:

protected:
	Vector3 m_position;

	// how much the entity is rotated around the y axis
	float m_yAngle;
public:
	GameObject3D(void);
	GameObject3D(float x, float y, float z);
	virtual ~GameObject3D(void);

	virtual void Update();
	virtual void Draw();

	inline Vector3 Position()
	{
		return m_position;
	}

	inline float X()
	{
		return m_position.X;
	}
	inline float Y()
	{
		return m_position.Y;
	}
	inline float Z()
	{
		return m_position.Z;
	}
	inline void SetX(float value)
	{
		m_position.X = value;
	}
	inline void SetY(float value)
	{
		m_position.Y = value;
	}
	inline void SetZ(float value)
	{
		m_position.Z = value;
	}
	inline float GetAngleY()
	{
		return m_yAngle;
	}
	inline void SetAngleY(float value)
	{
		m_yAngle = value;
	}
	// rotate around the y axis
	//void RotateY(float angle);
};

#endif
