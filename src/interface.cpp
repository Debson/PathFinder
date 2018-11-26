#include "interface.h"

#include <iostream>

#include "input.h"
#include "conf.h"
#include "grid.h"

namespace md 
{
	namespace interface
	{
		EditMode m_CurrentEditState = EditMode::None;

		const input::KeyCode NoneKeyCode = input::KeyCode::Escape;
		const input::KeyCode WallKeyCode = input::KeyCode::W;
		const input::KeyCode StartKeyCode = input::KeyCode::S;
		const input::KeyCode FinishKeyCode = input::KeyCode::F;
		const input::KeyCode EraseKeyCode = input::KeyCode::E;

		float m_GridScale = GRID_START_SCALE;

	};

	void interface::Start()
	{

	}

	void interface::ProcessInput()
	{
		// Find out what edit mode
		if (input::IsKeyPressed(NoneKeyCode) == true)
			m_CurrentEditState = EditMode::None;
		else if(input::IsKeyPressed(WallKeyCode) == true)
			m_CurrentEditState = EditMode::Wall;
		else if(input::IsKeyPressed(EraseKeyCode) == true || input::IsKeyPressed(input::KeyCode::MouseRight) == true)
			m_CurrentEditState = EditMode::Erase;
		else if (input::IsKeyPressed(StartKeyCode) == true)
			m_CurrentEditState = EditMode::Start;
		else if (input::IsKeyPressed(FinishKeyCode) == true)
			m_CurrentEditState = EditMode::Finish;


		if (grid::GridMap::IsRendering() == false)
		{
			// Edit scale of gird
			if (input::IsScrollForwardActive() == true)
			{
				m_GridScale += GRID_SCALE_STEP;
				if (m_GridScale > GRID_MAX_SCALE)
					m_GridScale = GRID_MAX_SCALE;
				grid::GridMap::UpdateGridScale(m_GridScale);
				//std::cout << m_GridScale << std::endl;

			}
			else if (input::IsScrollBackwardActive() == true)
			{
				m_GridScale -= GRID_SCALE_STEP;
				if (m_GridScale < GRID_MIN_SCALE)
					m_GridScale = GRID_MIN_SCALE;
				grid::GridMap::UpdateGridScale(m_GridScale);
				//std::cout << m_GridScale << std::endl;
			}
		}
	}

	interface::EditMode interface::GetEditState()
	{

		return m_CurrentEditState;
	}
}