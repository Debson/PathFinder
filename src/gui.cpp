#include "gui.h"

#include <glad/glad.h>
#include "shader_conf.h"
#include "path_finder.h"
#include "interface.h"


namespace md
{
	namespace gui
	{
		float lineWidth = 1;
		std::string path = "Path Length: ";
		int speed = 50;
		bool diagonal = true;
		bool renderPath = true;
		bool renderAttemps = true;
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

		const char *algorithms[] = { A_STAR_ALGORITHM, BREADTH_FIRST_SEARCH, DIJKSTRA_ALGORITHM };
		static const char *current_algo =  algorithms[0];
		if(ImGui::BeginCombo("Algorithm", current_algo))
        {
		    for(auto & algorithm : algorithms)
            {
		        bool is_selected = (current_algo == algorithm);
		        if(ImGui::Selectable(algorithm, is_selected))
		            current_algo = algorithm;
		        if(is_selected)
		            ImGui::SetItemDefaultFocus();
            }

            grid::GridMap::SetAlgorithm(current_algo);

		    ImGui::EndCombo();
        }

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
			ImGui::SliderInt("Speed", &speed, 0, 100);
            grid::GridMap::SetRenderSpeed(speed);
		}

		if (ImGui::Checkbox("Show Path", &renderPath))
		{
			grid::GridMap::SetRenderPath(renderPath);

		}
		ImGui::SameLine(210.f);

		if (ImGui::Checkbox("Show Attempts", &renderAttemps))
		{
            grid::GridMap::SetRenderAttempts(renderAttemps);
		}

		if (ImGui::Button("FindPath"))
		{
			grid::GridMap::ClearGridPathAndAttempts();

			path = "Path Length: ";
			int res = grid::GridMap::SolveGrid();
			if (res == -1)
			{
				path += "Could not find a path";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
					"Path not found!",
					"Path could not be find. Please make sure that grid is drawn correctly.",
					NULL);
			}
			else if (res == -2)
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
					"Start/Finish not assigned!",
					"Start or/and Finish point is not assigned. Please make sure Start and Finish is assigned on the grid.",
					NULL);
			}
			else
				path += std::to_string(res);

			if(showSteps)
				grid::GridMap::SetRenderSpeed(abs(100 - speed));
		}

		ImGui::Text(("Calculations Time: " + std::to_string(grid::GridMap::GetCalculationsTime()) + "ms").c_str());
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