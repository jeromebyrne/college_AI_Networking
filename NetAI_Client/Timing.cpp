#include "precompiled.h"
#include "Timing.h"

Timing * Timing::m_instance = 0;

Timing * Timing::GetInstance()
{
	if(m_instance == 0)
	{
		m_instance = new Timing();
	}
	return m_instance;
}

Timing::Timing(void):m_elapsed(0)
{
}

Timing::~Timing(void)
{
}
void Timing::LockFrameRate(int ticksPerSecond)
{
	// control frame rate
	timeBeginPeriod(1); // set timing resolution to 1 millisecond
	static long old_time=0,new_time=0;
	static int  since_last_update=0;
	static int num_frames=0;

	long elapsed;
	new_time=timeGetTime();
	elapsed=new_time-old_time;
	while(elapsed< 1000/ticksPerSecond){//frame rate 
		Sleep(1);
		new_time=timeGetTime();
		elapsed=new_time-old_time;
	}
	m_elapsed = elapsed;
	num_frames++;
	old_time=new_time;

	// display the frame rate
	since_last_update+=elapsed;
	if(since_last_update>1000){

		float frame_rate=1000.0f/(float)elapsed;
		GLwindow::GetInstance()->SetTitle("Frame rate: %4f", frame_rate);
		since_last_update=0;
	}
}
