#include "grid.h"

#include <iostream>

#include "conf.h"
#include "shader_conf.h"


namespace md
{
	grid::Grid::Grid()
	{

	}

	grid::Grid::~Grid()
	{

	}

	void grid::Grid::Start()
	{

	}

	void grid::Grid::Update()
	{

	}

	void grid::Grid::Render()
	{

	}


	void grid::Grid::OnWindowResize(glm::ivec2 newDim)
	{
		int lineCountHori = (float)newDim.y / GRID_SIZE;
		float step = 2.f / lineCountHori;

		float *newVertices = new float[lineCountHori * 2];
		float curr = 1.f;
		for (int i = 0; i < lineCountHori * 2; i += 2)
		{
			newVertices[i] = -1.f;
			newVertices[i + 1] = curr;
			curr -= step;
			//std::cout << newVertices[i] << " : " << newVertices[i + 1] << std::endl;
		}

		shader::UpdateGridHorizontal(newVertices, lineCountHori);
		delete[] newVertices;
		newVertices = nullptr;

		
		int lineCountVert = (float)newDim.x / GRID_SIZE;
		step = 2.f / lineCountVert;

		newVertices = new float[lineCountVert * 2];
		curr = -1.f;
		for (int i = 0; i < lineCountVert * 2; i += 2)
		{
			newVertices[i] = curr;
			newVertices[i + 1] = 1.0;
			curr += step;
			//std::cout << newVertices[i] << " : " << newVertices[i + 1] << std::endl;
		}

		shader::UpdateGridVertical(newVertices, lineCountVert);

		delete[] newVertices;
	}
}