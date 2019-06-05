#include "timer.h"

#include <SDL2/SDL.h>
#undef main

namespace md
{
	timer::Timer::Timer() : m_StartTicks(0), m_TargetTime(0), m_Started(false), m_Finished(false), m_Initialized(false) { };

	timer::Timer::Timer(int tt) : m_StartTicks(0), m_TargetTime(tt), m_Started(false), m_Finished(false), m_Initialized(false) { }

	void timer::Timer::Start()
	{
		m_Started = true;
		m_StartTicks = SDL_GetTicks();
	}

	void timer::Timer::Restart()
	{
		m_StartTicks = SDL_GetTicks();
	}

	void timer::Timer::ChangeTargetTime(int newtt)
	{
		m_TargetTime = newtt;
	}

	int timer::Timer::GetTicks()
	{
		return SDL_GetTicks() - m_StartTicks;
	}

	bool timer::Timer::isFinished()
	{
		return (SDL_GetTicks() - m_StartTicks > m_TargetTime);
	}
}