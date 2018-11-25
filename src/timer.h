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
			Timer(uint32_t tt);


			void Start();
			void Restart();
			void ChangeTargetTime(uint32_t newtt);
				
			bool isFinished();
		private:
			uint32_t m_StartTicks;
			uint32_t m_TargetTime;
			bool m_Started;
			bool m_Finished;
			bool m_Stopped;
			bool m_Initialized;
		};


	}
}
#endif // !TIMER_H
