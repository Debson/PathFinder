#ifndef GRID_H
#define GRID_H

#include <vector>

#include <boost/multi_array.hpp>
#include <glm.hpp>

#include "conf.h"


//TODO: create grid struct, put in in vector
namespace md
{
namespace grid
{
	enum CellType
	{
		Start,
		Finish,
		Wall,
		Step,
		Path,
		None

	};

	struct Grid
	{
		uint32_t row;
		uint32_t column;
	};

	struct GridDataType
	{
		GridDataType() : x(0), y(0), type(CellType::None) { };
		~GridDataType() { type = CellType::None; };
		uint32_t x;
		uint32_t y;
		CellType type;
	};


	struct SavedGridData : Grid
	{
		CellType type;
	};

	struct VertexData
	{
		glm::vec2 translation;
		glm::vec3 color;
	};

	struct GridStartFinish : Grid
	{
		GridStartFinish() : assigned(false) { };
		bool assigned;
	};

	struct SavedGridSummary
	{
		GridStartFinish start;
		GridStartFinish finish;
		std::vector<SavedGridData> grid_data;
		// Vector with all translations that are send as vertex data
		std::vector<VertexData> translations;
		// Reference vector that stores row and column of every translation (translation_ref[n] has column and row of translation[n])
		std::vector<Grid> translations_ref;
	};

	struct Point
	{
		int x;
		int y;
	};

	struct queueNode
	{
		Point pt;  // The cordinates of a cell 
		int dist;  // cell's distance of from the source 
	};

	const int rowNum[] = { -1, 0, 0, 1 };
	const int colNum[] = { 0, -1, 1, 0 };


	class GridMap
	{
	public:
		GridMap();
		~GridMap();

		void Start();
		void Update();
		void Render();
		static GridDataType *GetGridAt(uint32_t x, uint32_t y);
		static void UpdateGridOnResize(uint16_t rows, uint16_t columns);
		static void UpdateGridOnDraw(uint32_t index, CellType cellType);
		static void UpdateGridScale(float s);
		void CheckCollision();
		static void OnWindowResize(glm::ivec2 newDim = m_GridDimensions);
		static int FindPath();
#ifdef _DEBUG_
		static void PrintGrid();
		static void ClearGrid();
#endif

	private:
		void EraseGrid(uint32_t i);
		static uint32_t GridSize();
		static bool isValid(uint32_t row, uint32_t col);

		static std::vector<GridDataType> m_Grid;
		static uint32_t m_GridRows, m_GridColumns;
		static SavedGridSummary m_SavedGridSummary;
		static glm::ivec2 m_GridDimensions;
		static void AssignGridData(uint32_t index, CellType cellType, glm::vec3 color);
		static float m_GridScale;


	};
}
}

#endif // !GRID_H
