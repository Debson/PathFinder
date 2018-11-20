#include "path_finder.h"

#include <iostream>

#include <GL/gl3w.h>
#include <gtc/matrix_transform.hpp>

#include "input.h"
#include "shader_conf.h"
#include "properties.h"

glm::ivec2 md::PathFinderApp::m_AppDimension = glm::ivec2(790, 600);

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

	SDL_GL_SetSwapInterval(1);

	m_Grid = new grid::GridMap();

	shader::Start(m_AppDimension);

	m_Grid->Start();
	m_Grid->OnWindowResize(m_AppDimension);

	isRunning = true;
	
#ifdef _DEBUG_
	m_imgui.Start();
#endif
}

void md::PathFinderApp::GameLoop()
{
	SDL_Event m_Event;

	while (isRunning == true)
	{
		OnNewFrame();

		ProcessInput(&m_Event);

		Update();

		Render();

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
	glPointSize(10.f);
	glLineWidth(0.0001f);

	GLfloat lineWidthRange[2] = { 0.0f, 0.0f };
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
	std::cout << lineWidthRange[0];

#ifdef _DEBUG_
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	io = ImGui::GetIO(); (void)io;
	ImGui_ImplSDL2_InitForOpenGL(m_Window, m_Context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();
#endif

	glViewport(0, 0, m_AppDimension.x, m_AppDimension.y);
}

void md::PathFinderApp::OnNewFrame()
{
	inputconf::StartNewFrame();

#ifdef _DEBUG_
	/* imgui */
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();
#endif
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
}

void md::PathFinderApp::Update()
{

}

void md::PathFinderApp::Render()
{
	glClearColor(1.f, 0.5f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::vec3 size(100.f);
	glm::vec2 pos(input::GetMousePosition().x - size.x / 2.f, input::GetMousePosition().y - size.y / 2.f);
	shader::Draw(pos, size, true);
	
	shader::DrawGrid();
	shader::DrawGrid(true);


#ifdef _DEBUG_
	m_imgui.Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

	SDL_GL_SwapWindow(m_Window);

	SDL_Delay(16);
}


glm::ivec2 &md::PathFinderApp::GetWindowSize()
{
	return m_AppDimension;
}
