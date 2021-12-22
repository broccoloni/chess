#pragma once 

class FPSLimiter
{
	public:
		FPSLimiter();
		~FPSLimiter();
		
		void init(float maxFPS);
		void setMaxFPS(float maxFPS);
		void begin();
		float end(); //end will return the current FPS	

	private:
		void calculateFPS();

		float m_fps;
		float m_maxFPS;
		float m_frameTime;
		unsigned int m_startTicks;
	

};




