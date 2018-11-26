#include "gui.h"


#include <GL/gl3w.h>

#include "shader_conf.h"
#include "path_finder.h"
#include "interface.h"


namespace md
{
	namespace gui
	{
		float lineWidth = 1;
		std::string path = "Path: ";
		int speed = 50;
		bool diagonal = false;
		bool renderPath = true;
		bool renderAttemps = false;
		bool showSteps = false;
		
	}

	gui::Imgui::Imgui()
	{
	}

	gui::Imgui::~Imgui()
	{

	}

	void gui::Imgui::Start()
	{

	}

	void gui::Imgui::Render()
	{
		ImGui::Begin("PathFinder");
#ifdef _DEBUG_
		if (ImGui::SliderFloat("Line width", &lineWidth, 1.f, 10.f))
		{
			glLineWidth(lineWidth);
		}
		if (ImGui::Button("Print grid"))
		{
			grid::GridMap::PrintGrid();
		}

		ImGui::Text(("Objects count: " + std::to_string(grid::GridMap::GetObjectsCount())).c_str());
#endif

		if (ImGui::Button("Clear grid"))
		{
			grid::GridMap::ClearGrid();
			path = "Path Length: ";
		}

		if (ImGui::Button("Clear path&attempts"))
		{
			grid::GridMap::ClearGridPathAndAttempts();
			path = "Path Length: ";
		}

		int i = ImGui::GetWindowSize().x;


		if (ImGui::Checkbox("Diagonal Moves Enabled", &diagonal))
		{
			grid::GridMap::SetDiagonal(diagonal);
		}

		if (ImGui::Checkbox("Show Steps", &showSteps))
		{
			grid::GridMap::SetShowSteps(showSteps);
			grid::GridMap::SetRenderSpeed(speed);
		}

		if (showSteps)
		{
			if (ImGui::SliderInt("Speed", &speed, 0, 100))
			{
				grid::GridMap::SetRenderSpeed(speed);
			}
		}
		
		if (ImGui::Checkbox("Show Path", &renderPath))
		{
			grid::GridMap::SetRenderPath(renderPath);

		}
		ImGui::SameLine(210.f);

		if (ImGui::Checkbox("Show Attemps", &renderAttemps))
		{
			grid::GridMap::SetRenderAttemps(renderAttemps);
		}

		if (ImGui::Button("FindPath"))
		{
			grid::GridMap::ClearGridPathAndAttempts();
			path = "Path Length: ";
			int res = grid::GridMap::SolveGrid();
			if (res < 0)
				path += "No path";
			else
				path += std::to_string(res);

			if(showSteps)
				grid::GridMap::SetRenderSpeed(speed);
		}
		ImGui::Text(path.c_str());

		std::string state = "Edit Mode: ";
		switch (interface::GetEditState())
		{
		case(interface::EditMode::None): {
			state += "None";
			break;
		}
		case(interface::EditMode::Wall): {
			state += "Wall";
			break;
		}
		case(interface::EditMode::Start): {
			state += "Start";
			break;
		}
		case(interface::EditMode::Finish): {
			state += "Finish";
			break;
		}
		case(interface::EditMode::Erase): {
			state += "Erase";
			break;
		}						  
		}

		ImGui::Text(state.c_str());

#ifdef _DEBUG_
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
#endif
		ImGui::End();
	}

}