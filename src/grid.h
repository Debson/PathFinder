#ifndef GRID_H
#define GRID_H

#include <vector>

#include <boost/multi_array.hpp>
#include <glm.hpp>

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
	typedef std::vector<std::pair<CellType, boost::multi_array<int, 2>>> GridData;

	class Grid
	{
	public:
		Grid();
		~Grid();

		void Start();
		void Update();
		void Render();
		void OnWindowResize(glm::ivec2 newDim);

	private:
		GridData m_Grid;

	};
}
}

#endif // !GRID_H
