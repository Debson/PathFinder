#include "path_finder.h"

#include <iostream>

#include <GL/gl3w.h>
#include <gtc/matrix_transform.hpp>

#include "input.h"
#include "shader_conf.h"
#include "properties.h"
#include "interface.h"
#include "timer.h"

glm::ivec2 md::PathFinderApp::m_AppDimension = glm::ivec2(790, 600);
SDL_Window *md::PathFinderApp::m_Window = NULL;

md::PathFinderApp::PathFinderApp()
{
}

md::PathFinderApp::~PathFinderApp()
{

}

void md::PathFinderApp::Start()
{
	SetupSDL();
	SetupOpenGL();
	SetupGlew();

	//SDL_GL_SetSwapInterval(1);

	m_Grid = new grid::GridMap();

	shader::Start(m_AppDimension);

	m_Grid->Start();
	m_Grid->OnWindowResize(m_AppDimension);
	isRunning = true;
	
	m_imgui.Start();
}

void md::PathFinderApp::GameLoop()
{
	SDL_Event m_Event;

	timer::Timer capTimer;

	while (isRunning == true)
	{
		capTimer.Start();
		OnNewFrame();

		ProcessInput(&m_Event);

		Update();

		Render();


		uint32_t frameTicks = capTimer.GetTicks();
		if (m_Grid->IsActive() == false && frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}

		OnFinishFrame();
	}
}


void md::PathFinderApp::Close()
{
	SDL_DestroyWindow(m_Window);
}

/* private methods */
void md::PathFinderApp::SetupSDL()
{
	m_Window = SDL_CreateWindow("PathFinder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_AppDimension.x, m_AppDimension.y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	//m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_CaptureMouse(SDL_FALSE);

}

void md::PathFinderApp::SetupOpenGL()
{
	this->m_Context = SDL_GL_CreateContext(m_Window);

	SDL_GL_MakeCurrent(m_Window, m_Context);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void md::PathFinderApp::SetupGlew()
{
	gl3wInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.f);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	io = ImGui::GetIO(); (void)io;
	ImGui_ImplSDL2_InitForOpenGL(m_Window, m_Context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();

	glViewport(0, 0, m_AppDimension.x, m_AppDimension.y);
}

void md::PathFinderApp::OnNewFrame()
{
	inputconf::StartNewFrame();

	/* imgui */
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();
}

void md::PathFinderApp::OnFinishFrame()
{
	m_Grid->SetActive(false);
}

void md::PathFinderApp::ProcessInput(SDL_Event* e)
{
	while (SDL_PollEvent(e) != 0)
	{
#ifdef _DEBUG_
		ImGui_ImplSDL2_ProcessEvent(e);
#endif

		switch (e->type)
		{
		case(SDL_QUIT): {
			isRunning = false;
			break;
		}
		case(SDL_MOUSEWHEEL): {
			inputconf::UpdateScrollPosition(e->wheel.x, e->wheel.y);
			break;
		}
		case(SDL_MOUSEMOTION): {
			inputconf::UpdateMousePosition(e->motion.x, e->motion.y);
			inputconf::UpdateRelativeMousePosition();
			m_Grid->CheckCollision();
			break;
		}
		}

		if (e->type == SDL_WINDOWEVENT)
		{
			switch (e->window.event)
			{
			case(SDL_WINDOWEVENT_RESIZED): {
				SDL_GetWindowSize(m_Window, &m_AppDimension.x, &m_AppDimension.y);
				shader::UpdateProjectionMatrix(m_AppDimension);
				m_Grid->OnWindowResize(m_AppDimension);
				glViewport(0, 0, m_AppDimension.x, m_AppDimension.y);
				break;
			}
			}
		}
	}

	const uint8_t *current_keystate = SDL_GetKeyboardState(NULL);
	inputconf::UpdateKeyState(current_keystate);

	const unsigned int current_mousestate = SDL_GetMouseState(NULL, NULL);
	inputconf::UpdateMouseState(current_mousestate);


	interface::ProcessInput();
}

void md::PathFinderApp::Update()
{
	m_Grid->Update();
}

void md::PathFinderApp::Render()
{
	glClearColor(GRID_COLOR.r, GRID_COLOR.g, GRID_COLOR.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw grid
	shader::DrawGrid();
	shader::DrawGrid(true);
	
	// Draw colored squares
	m_Grid->Render();

	m_imgui.Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_Window);
}


glm::ivec2 &md::PathFinderApp::GetWindowSize()
{
	return m_AppDimension;
}

SDL_Window *md::PathFinderApp::GetSDLWindow()
{
	return m_Window;
}
