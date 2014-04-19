#pragma once

class Timing
{
private:
	int m_elapsed;
public:
	static Timing * m_instance;
	Timing(void);
	~Timing(void);
	static Timing * GetInstance();
	void LockFrameRate(int ticksPerSecond);
	inline float Elapsed()
	{
		float value = (float)m_elapsed;

		if(value > 1000)
		{
			value = 0;
		}
		return value/1000;
	}
};
