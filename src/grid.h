#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <stack>

#include <glm/glm.hpp>

#include "conf.h"
#include "timer.h"


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
	    Grid() { }
	    Grid(int r, int c) : row(r), column(c) { };
		int row;
		int column;
	};

    struct GridAStar : public Grid
	{
		double overall_cost, cost_to_start, cost_to_finish;
	};

	struct SavedGridData : Grid
	{
	    SavedGridData() { }
	    SavedGridData(int r, int c, CellType t) : type(t) { this->row = r; this->column = c;}
		CellType type;
	};

	struct VertexData
	{
	    VertexData() { };
	    VertexData(glm::vec2 t, glm::vec3 c) : translation(t), color(c) { };
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
		PathRenderData(std::vector<Grid> p) : counter(0), pairs(p), render(true)
		{ 
			step = pairs.size() * 0.01f;
			if (step <= 0)
				step = 1;
			timer = timer::Timer(GRID_BASE_RENDER_DELAY); 
			timer.Start();
		};
		std::vector<Grid> pairs;
		int counter;
		int step;
		timer::Timer timer;
		bool render;
	};

	struct AttemptsRenderData
	{
		AttemptsRenderData() { };
		AttemptsRenderData(std::vector<Grid> p) : counter(0), pairs(p), render(true)
		{
			step = pairs.size() * 0.01f;
			if (step <= 0)
				step = 1;
			timer = timer::Timer(GRID_BASE_RENDER_DELAY);
			timer.Start();
		};
		std::vector<Grid> pairs;
		int counter;
		int step;
		timer::Timer timer;
		bool render;
	};

	struct SavedGridSummary
	{
		GridStartFinish start;
		GridStartFinish finish;
		std::vector<SavedGridData> grid_data;
		// Actual grid reprezentation
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
		static void UpdateGridOnDraw(int index, CellType cellType);
		static void UpdateGridScale(float s);
		void CheckCollision();
		static void OnWindowResize(glm::ivec2 newDim = m_GridDimensions);
		static int SolveGrid();
		static void SetDiagonal(bool val);
		static void SetRenderPath(bool val);
		static void SetRenderAttempts(bool val);
		static void SetShowSteps(bool val);
		static void SetRenderSpeed(int speed);
        static void SetAlgorithm(const char *algoName);
		static void ClearGrid();
		static void ClearGridPathAndAttempts();
		static double CalculateHeuristics(int row, int column);
        static void TracePath(GridAStar grid[], std::vector<Grid> *path);

		static bool IsActive();
		static void SetActive(bool val);
		static bool IsRendering();
		static int GetCalculationsTime();


#ifdef _DEBUG_
		static void PrintGrid();
		static int GetObjectsCount();
#endif

	private:;
	    static void EraseGrid(int row, int col);
		static int GridSize();
		static bool isValid(int row, int col);
		static void AssignGridData(int index, CellType cellType, glm::vec3 color);
		static void OnGridSolved(std::vector<Grid> *path, std::vector<Grid> *attempts);

		static int m_GridRows, m_GridColumns;
		static SavedGridSummary m_SavedGridSummary;
		static glm::ivec2 m_GridDimensions;
		static float m_GridScale;
		static bool m_Diagonal;
		static bool m_RenderPath;
		static bool m_RenderAttemps;
		static bool m_ShowSteps;
		static bool m_Active;
		static int m_RenderSpeed;
		static int m_CalculationsTime;
		static const char * m_CurrentAlgorithm;
		




	};
}
}

#endif // !GRID_H
