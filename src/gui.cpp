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
		ImGui::Begin("_DEBUG_");
		if (ImGui::SliderFloat("Line width", &lineWidth, 1.f, 10.f))
		{
			glLineWidth(lineWidth);
		}

		/*if (ImGui::Button("Update lines"))
		{
			float space = 20.f;
			int lineCount = (float)PathFinderApp::GetWindowSize().y / space;
			float step = 2.f / lineCount;

			float *newVertices = new float[lineCount * 2];
			float curr = 1.f;
			for (int i = 0; i < lineCount * 2; i += 2)
			{
				newVertices[i] = -1.f;
				newVertices[i + 1] = curr;
				curr -= step;
				//std::cout << newVertices[i] << " : " << newVertices[i + 1] << std::endl;
			}
			
			shader::UpdateGridHorizontal(newVertices, lineCount);
			delete newVertices;
		}*/

		if (ImGui::Button("Print grid"))
		{
			grid::GridMap::PrintGrid();
		}

		if (ImGui::Button("Clear grid"))
		{
			grid::GridMap::ClearGrid();
			path = "Path: ";
		}

		if (ImGui::Button("FindPath"))
		{
			path += std::to_string(grid::GridMap::FindPath());
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

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}