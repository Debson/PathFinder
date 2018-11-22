#include "input.h"

namespace md
{
	static bool mdNeedsInitializaiotn(true);
	static bool mdCurrentKeyStatus[input::KeyCode::count];
	static bool mdPreviousKeyStatus[input::KeyCode::count];

	static int mdCurrentMouseX, mdCurrentMouseY;
	static int mdRelativeMouseX, mdRelativeMouseY;
	static int mdPreviousMouseX, mdPreviousMouseY;

	static bool mdIsMouseScrollActive(false);
	static int mdCurrentScrollX, mdCurrentScrollY;
	static int mdPreviousScrollX, mdPreviousScrollY;


	bool input::IsKeyPressed(const KeyCode& key)
	{
		return (mdCurrentKeyStatus[key] == true && mdPreviousKeyStatus[key] == false) ? true : false;
	}

	bool input::IsKeyReleased(const KeyCode& key)
	{
		return (mdCurrentKeyStatus[key] == false && mdPreviousKeyStatus[key] == true) ? true : false;
	}

	bool input::IsKeyDown(const KeyCode& key)
	{
		return mdCurrentKeyStatus[key];
	}

	bool input::IsMouseActive()
	{
		return ((mdCurrentMouseX - mdPreviousMouseX) != 0 || (mdCurrentMouseY - mdPreviousMouseY) != 0) ? true : false;
	}

	bool input::IsScrollForwardActive()
	{
		return (mdIsMouseScrollActive == true && mdCurrentScrollY > 0) ? true : false;
	}

	bool input::IsScrollBackwardActive()
	{
		return (mdIsMouseScrollActive == true && mdCurrentScrollY < 0) ? true : false;
	}

	bool input::IsScrollActive()
	{
		return mdIsMouseScrollActive == true ? true : false;
	}

	glm::vec2 input::GetMousePosition()
	{
		return glm::vec2(mdCurrentMouseX, mdCurrentMouseY);
	}

	void input::GetMousePosition(int *x, int *y)
	{
		*x = mdCurrentMouseX;
		*y = mdCurrentMouseY;
	}

	glm::vec2 input::GetGlobalMousePosition()
	{
		int x, y;
		SDL_GetGlobalMouseState(&x, &y);
		return glm::vec2(x, y);
	}

	glm::vec2 input::GetRelativeMousePosition()
	{
		return glm::vec2(mdRelativeMouseX, mdRelativeMouseY);
	}

	/* ******* INPUT CONFIGURATION ******** */
	void inputconf::OnPressKey(const input::KeyCode& key)
	{
		mdCurrentKeyStatus[key] = true;
	}

	void inputconf::OnReleaseKey(const input::KeyCode& key)
	{
		mdCurrentKeyStatus[key] = false;
	}

	void inputconf::StartNewFrame(void)
	{
		if (mdNeedsInitializaiotn == true)
		{
			mdNeedsInitializaiotn = false;
			for (uint8_t keyIndex = 0; keyIndex < input::KeyCode::count; ++keyIndex)
			{
				mdCurrentKeyStatus[keyIndex] = false;
			}
		}

		for (uint8_t keyIndex = 0; keyIndex < input::KeyCode::count; ++keyIndex)
		{
			mdPreviousKeyStatus[keyIndex] = mdCurrentKeyStatus[keyIndex];
		}

		mdPreviousMouseX = mdCurrentMouseX;
		mdPreviousMouseY = mdCurrentMouseY;

		mdPreviousScrollX = mdCurrentScrollX;
		mdPreviousScrollY = mdCurrentScrollY;
		mdIsMouseScrollActive = false;
	}

	void inputconf::UpdateRelativeMousePosition()
	{
		SDL_GetRelativeMouseState(&mdRelativeMouseX, &mdRelativeMouseY);
	}

	void inputconf::UpdateMousePosition(int mouseX, int mouseY)
	{
		mdCurrentMouseX = mouseX;
		mdCurrentMouseY = mouseY;
	}

	void inputconf::UpdateScrollPosition(int scrollX, int scrollY)
	{
		mdCurrentScrollX = scrollX;
		mdCurrentScrollY = scrollY;
		mdIsMouseScrollActive = true;
	}

	void inputconf::UpdateKeyState(const uint8_t* state)
	{
		for (uint8_t keyIndex = 0; keyIndex < input::KeyCode::count; ++keyIndex)
		{
			mdCurrentKeyStatus[keyIndex] = state[keyIndex];
		}
	}

	void inputconf::UpdateMouseState(const uint8_t state)
	{
		mdCurrentKeyStatus[input::KeyCode::MouseLeft] = state & SDL_BUTTON(SDL_BUTTON_LEFT);
		mdCurrentKeyStatus[input::KeyCode::MouseRight] = state & SDL_BUTTON(SDL_BUTTON_RIGHT);
		mdCurrentKeyStatus[input::KeyCode::MouseMiddle] = state & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	}
}