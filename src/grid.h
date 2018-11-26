#ifndef GRID_H
#define GRID_H

#include <vector>

#include <boost/multi_array.hpp>
#include <glm.hpp>

#include "conf.h"
#include "timer.h"


//TODO: create grid struct, put in in vector
namespace md
{
namespace grid
{
	enum CellType
	{
		None,
		Wall,
		Start,
		Finish,
		Attempt,
		Path,

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

	struct PathRenderData
	{
		PathRenderData() { };
		PathRenderData(std::vector<std::pair<uint32_t, uint32_t>> p) : counter(0), pairs(p), render(true)
		{ 
			step = pairs.size() * 0.01f;
			if (step <= 0)
				step = 1;
			timer = timer::Timer(GRID_BASE_RENDER_DELAY); 
			timer.Start();
		};
		std::vector<std::pair<uint32_t, uint32_t>> pairs;
		uint32_t counter;
		uint32_t step;
		timer::Timer timer;
		bool render;
	};

	struct AttemptsRenderData
	{
		AttemptsRenderData() { };
		AttemptsRenderData(std::vector<std::pair<uint32_t, uint32_t>> p) : counter(0), pairs(p), render(true)
		{
			step = pairs.size() * 0.01f;
			if (step <= 0)
				step = 1;
			timer = timer::Timer(GRID_BASE_RENDER_DELAY);
			timer.Start();
		};
		std::vector<std::pair<uint32_t, uint32_t>> pairs;
		uint32_t counter;
		uint32_t step;
		timer::Timer timer;
		bool render;
	};

	struct SavedGridSummary
	{
		GridStartFinish start;
		GridStartFinish finish;
		std::vector<SavedGridData> grid_data;
		// Actual grid reprezentation
		std::vector<uint32_t> grid_rep;
		// Vector with all translations that are send as vertex data
		std::vector<VertexData> translations;
		// Reference vector that stores row and column of every translation (translation_ref[n] has column and row of translation[n])
		std::vector<Grid> translations_ref;
		PathRenderData pathRenderData;
		AttemptsRenderData attemptsRenderData;

	};

	class GridMap
	{
	public:
		GridMap();
		~GridMap();

		void Start();
		void Update();
		void Render();
		static void UpdateGridOnResize(uint16_t rows, uint16_t columns);
		static void UpdateGridOnDraw(uint32_t index, CellType cellType);
		static void UpdateGridScale(float s);
		void CheckCollision();
		static void OnWindowResize(glm::ivec2 newDim = m_GridDimensions);
		static int SolveGrid();
		static uint32_t *GetGridAt(uint32_t row, uint32_t col);
		static void SetDiagonal(bool val);
		static void SetRenderPath(bool val);
		static void SetRenderAttemps(bool val);
		static void SetShowSteps(bool val);
		static void SetRenderSpeed(uint32_t speed);
		static void ClearGrid();
		static void ClearGridPathAndAttempts();
		static bool IsActive();
		static void SetActive(bool val);
		static bool IsRendering();
		static uint32_t GetCalculationsTime();
#ifdef _DEBUG_
		static void PrintGrid();
		static int GetObjectsCount();
#endif

	private:
		static bool EraseGrid(uint32_t i);
		static void EraseGrid(CellType cellType);
		static uint32_t GridSize();
		static bool isValid(uint32_t row, uint32_t col);
		static void AssignGridData(uint32_t index, CellType cellType, glm::vec3 color);
		static void OnGridSolved(std::vector<std::pair<uint32_t, uint32_t>> *path, std::vector<std::pair<uint32_t, uint32_t>> *attempts);

		static uint32_t m_GridRows, m_GridColumns;
		static SavedGridSummary m_SavedGridSummary;
		static glm::ivec2 m_GridDimensions;
		static float m_GridScale;
		static bool m_Diagonal;
		static bool m_RenderPath;
		static bool m_RenderAttemps;
		static bool m_ShowSteps;
		static bool m_Active;
		static int m_RenderSpeed;
		static uint32_t m_CalculationsTime;
		




	};
}
}

#endif // !GRID_H
