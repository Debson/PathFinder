#include "grid.h"

#include <iostream>

#include "conf.h"
#include "shader_conf.h"


namespace md
{

	std::vector<grid::GridData> grid::GridMap::m_Grid;

	grid::GridMap::GridMap()
	{

	}

	grid::GridMap::~GridMap()
	{

	}

	void grid::GridMap::Start()
	{
		
	}

	void grid::GridMap::Update()
	{

	}

	void grid::GridMap::Render()
	{

	}
	/*	min c, r: 1 */
	grid::GridData *grid::GridMap::GetGridAt(uint32_t r, uint32_t c)
	{
		// TODO
		return &m_Grid[1];
	}

	uint32_t grid::GridMap::GetGridSize()
	{
		return m_Grid.size();
	}

	void grid::GridMap::UpdateGrid(uint16_t rows, uint16_t columns)
	{
		m_Grid.clear();
		m_Grid.resize(rows * columns);
		uint32_t size = rows * columns;
		for (uint32_t i = 0; i < size; i++)
		{
			// TODO
		}

	}

	void grid::GridMap::OnWindowResize(glm::ivec2 newDim)
	{
		int add = newDim.y % GRID_SIZE;
		int size = newDim.y + (add > 0 ? GRID_SIZE - add : 0);
		int lineCountHori = size / GRID_SIZE;
		float step = (2.f * (size / (float)newDim.y)) / lineCountHori;

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

		add = newDim.x % GRID_SIZE;
		size = newDim.x + (add > 0 ? GRID_SIZE - add : 0);
		int lineCountVert = size / GRID_SIZE;
		step = (2.f * (size / (float)newDim.x)) / lineCountVert;

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