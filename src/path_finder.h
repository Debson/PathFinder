#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <SDL.h>
#undef main

#include <glm.hpp>
#include "conf.h"
#include "gui.h"
#include "grid.h"

namespace md
{
	class PathFinderApp
	{
	public:
		PathFinderApp();
		~PathFinderApp();

		void Start();
		void GameLoop();
		void Close();

		static glm::ivec2 &GetWindowSize();
		static SDL_Window* GetSDLWindow();

	private:
		void SetupSDL();
		void SetupOpenGL();
		void SetupGlew();
		void OnNewFrame();
		void ProcessInput(SDL_Event *e);
		void Update();
		void Render();

		static SDL_Window *m_Window;
		SDL_GLContext m_Context;
		grid::GridMap* m_Grid;


		static glm::ivec2 m_AppDimension;
		

		bool isRunning;

		ImGuiIO io;
		const char* glsl_version = "#version 130";
		gui::Imgui m_imgui;
	};
}

#endif // !
