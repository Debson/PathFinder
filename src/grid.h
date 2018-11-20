#ifndef GRID_H
#define GRID_H

#include <vector>

#include <boost/multi_array.hpp>
#include <glm.hpp>
//TODO: create grid struct, put in in vector
namespace md
{
namespace grid
{
	enum CellType
	{
		Start,
		End,
		Wall,
		None

	};


	struct GridData
	{
		uint32_t x;
		uint32_t y;
		CellType type;


	};


	class GridMap
	{
	public:
		GridMap();
		~GridMap();

		void Start();
		void Update();
		void Render();
		static GridData *GetGridAt(uint32_t x, uint32_t y);
		static uint32_t GetGridSize();
		static void UpdateGrid(uint16_t rows, uint16_t columns);
		void OnWindowResize(glm::ivec2 newDim);

	private:
		static std::vector<GridData> m_Grid;

	};
}
}

#endif // !GRID_H
