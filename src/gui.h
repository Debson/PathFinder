#ifndef GUI_H
#define GUI_H

#include "conf.h"

#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_sdl.h"
#include "../../external/imgui/imgui_impl_opengl3.h">

namespace md
{
namespace gui
{
	class Imgui
	{
	public:
		Imgui();
		~Imgui();

		void Start();
		void Render();

	private:
	};
}
}

#endif // !GUI_H
