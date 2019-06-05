#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

namespace md
{
	namespace timer
	{

		class Timer
		{
		public:
			Timer();
			Timer(int tt);


			void Start();
			void Restart();
			void ChangeTargetTime(int newtt);
			int GetTicks();
				
			bool isFinished();
		private:
			int m_StartTicks;
			int m_TargetTime;
			bool m_Started;
			bool m_Finished;
			bool m_Stopped;
			bool m_Initialized;
		};


	}
}
#endif // !TIMER_H
