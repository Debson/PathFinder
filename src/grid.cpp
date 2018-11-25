#include "grid.h"

#include <iostream>
#include <queue>
#include <vector>

#include "shader_conf.h"
#include "input.h"
#include "interface.h"
#include "path_finder.h"

namespace md
{
	namespace grid
	{
		struct queueNode
		{
			Grid pt;  // The cordinates of a cell 
			int dist;  // cell's distance of from the source 
			std::vector<std::pair<uint32_t, uint32_t>> pairs;

		};


		std::vector<int8_t> rowNum = { -1, 0, 0, 1, -1,  1, 1, -1 };
		std::vector<int8_t> colNum = { 0, -1, 1, 0, -1, -1, 1,  1 };

		uint32_t start = 0;

	}

	std::vector<grid::GridDataType> grid::GridMap::m_Grid;
	
	uint32_t grid::GridMap::m_GridRows			= 0;
    grid::SavedGridSummary grid::GridMap::m_SavedGridSummary;
	uint32_t grid::GridMap::m_GridColumns		= 0;
	glm::ivec2 grid::GridMap::m_GridDimensions;
	float grid::GridMap::m_GridScale			= 1.f;
	bool grid::GridMap::m_Diagonal				= false;
	bool grid::GridMap::m_RenderPath			= true;
	bool grid::GridMap::m_RenderAttemps			= false;
	bool grid::GridMap::m_ShowSteps				= false;
	int grid::GridMap::m_RenderSpeed			= 100;

	grid::GridMap::GridMap() { }

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
		// Draw path if registered
		if(m_SavedGridSummary.pathRenderData.render == true && m_SavedGridSummary.pathRenderData.timer.isFinished() == true && m_SavedGridSummary.attemptsRenderData.render == false)
		{ 
			auto pair = m_SavedGridSummary.pathRenderData.pairs[m_SavedGridSummary.pathRenderData.counter];
			UpdateGridOnDraw(pair.first * m_GridColumns + pair.second, grid::CellType::Path);
			m_SavedGridSummary.pathRenderData.counter++;
			m_SavedGridSummary.pathRenderData.timer.Restart();

			if (m_SavedGridSummary.pathRenderData.counter > m_SavedGridSummary.pathRenderData.pairs.size() - 1)
				m_SavedGridSummary.pathRenderData.render = false;
		}

		// Draw attemps if registered
		if (m_SavedGridSummary.attemptsRenderData.render == true && m_SavedGridSummary.attemptsRenderData.timer.isFinished() == true)
		{
			auto pair = m_SavedGridSummary.attemptsRenderData.pairs[m_SavedGridSummary.attemptsRenderData.counter];
			UpdateGridOnDraw(pair.first * m_GridColumns + pair.second, grid::CellType::Attempt);
			m_SavedGridSummary.attemptsRenderData.counter++;
			m_SavedGridSummary.attemptsRenderData.timer.Restart();

			if (m_SavedGridSummary.attemptsRenderData.counter > m_SavedGridSummary.attemptsRenderData.pairs.size() - 1)
				m_SavedGridSummary.attemptsRenderData.render = false;
		}

		// Draw colored squares
		shader::Draw();

	}
	/*	min c, r: 1 */
	grid::GridDataType *grid::GridMap::GetGridAt(uint32_t r, uint32_t c)
	{
		if (r == 0)
			return &m_Grid[c];
		
		if (r > 0 && c == 0)
			return &m_Grid[r * m_GridColumns];
		if (r * m_GridColumns + c >= m_GridColumns * m_GridRows)
			return &m_Grid[m_GridColumns * m_GridRows - 1];

		return &m_Grid[r * m_GridColumns + c];
	}

	void grid::GridMap::UpdateGridOnResize(uint16_t rows, uint16_t columns)
	{
		m_GridRows = rows;
		m_GridColumns = columns;

		m_Grid.clear();
		m_Grid.resize(rows * columns);

		float sizendcX = (float)GridSize() / (float)m_GridDimensions.x * 2.f;
		float sizendcY = (float)GridSize() / (float)m_GridDimensions.y * 2.f;

		m_SavedGridSummary.translations.clear();
		m_SavedGridSummary.translations_ref.clear();
		for(auto & i : m_SavedGridSummary.grid_data)
		{
			if (i.row <= rows && i.column <= columns)
			{
				GetGridAt(i.row, i.column)->type = i.type;
				
				VertexData vertData;
				vertData.translation = glm::vec2(sizendcX * i.column, sizendcY * i.row * -1);
				glm::vec3 color;
				switch (i.type)
				{
				case(CellType::None): {
				break;
				}
				case(CellType::Wall): {
					color = GRID_WALL_COLOR;
					break;
				}
				case(CellType::Start): {
					color = GRID_START_COLOR;
					break;
				}
				case(CellType::Finish): {
					color = GRID_FINISH_COLOR;
					break;
				}
				case(CellType::Attempt): {
					color = GRID_ATTEMPT_COLOR;
					break;
				}
				case(CellType::Path): {
					color = GRID_PATH_COLOR;
					break;
				}
				}
				vertData.color = color;
				m_SavedGridSummary.translations.push_back(vertData);
				Grid transData;
				transData.row = i.row;
				transData.column = i.column;
				m_SavedGridSummary.translations_ref.push_back(transData);
			}
		}
		

		/*float sizendcX = 0.05f;
		float sizendcY = 0.05f;*/


		float squareVertices[12] =
		{
			-1.00f - sizendcX, 1.00f,
			-1.00f, 1.00f - sizendcY,
			-1.00f - sizendcX, 1.00f - sizendcY,

			-1.00f - sizendcX, 1.00f,
			-1.00f, 1.00f - sizendcY,
			-1.00f, 1.00f
		};


		shader::UpdateShader(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size(), squareVertices);
	}

	void grid::GridMap::UpdateGridOnDraw(uint32_t index, grid::CellType cellType)
	{
		switch (cellType)
		{
		case(CellType::None): {
				break;
			}
		case(CellType::Wall): {
			int row = index / m_GridColumns;
			int column = index % m_GridColumns + 1;
			// check if already exists
			auto it = std::find_if(m_SavedGridSummary.grid_data.begin(), m_SavedGridSummary.grid_data.end(),
				[&](const grid::SavedGridData & ref) { return ref.row == row && ref.column == column; });
			if (it != m_SavedGridSummary.grid_data.end())
			{
				return;
			}
			AssignGridData(index, cellType, GRID_WALL_COLOR);
			break;
		}
		case(CellType::Start): {
			AssignGridData(index, cellType, GRID_START_COLOR);
			m_SavedGridSummary.start.assigned = true;
			break;
		}
		case(CellType::Finish): {
			AssignGridData(index, cellType, GRID_FINISH_COLOR);
			m_SavedGridSummary.finish.assigned = true;
			break;
		}
		case(CellType::Attempt): {
			
			AssignGridData(index, cellType, GRID_ATTEMPT_COLOR);
			
			break;
		}
		case(CellType::Path): {
			AssignGridData(index, cellType, GRID_PATH_COLOR);
			
			break;
		}
		}

		shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());
	}

	void grid::GridMap::UpdateGridScale(float s)
	{
		m_GridScale = s;
		OnWindowResize();
	}

	void grid::GridMap::CheckCollision()
	{
		if (input::IsKeyDown(input::KeyCode::MouseLeft) == false)
			return;
		int x, y;
		input::GetMousePosition(&x, &y);
		if (x > m_GridDimensions.x || x < 0 || y >= m_GridDimensions.y || y < 0)
			return;
		uint32_t i = x / GridSize();
		i += y / GridSize() * m_GridColumns;
		
		
		switch (interface::GetEditState())
		{
		case(interface::EditMode::Wall): {
			m_Grid[i].type = grid::CellType::Wall;
			UpdateGridOnDraw(i, grid::CellType::Wall);
			break;
		}
		case(interface::EditMode::Start): {
			if (m_SavedGridSummary.start.assigned == true)
				break;
			m_Grid[i].type = grid::CellType::Start;
			UpdateGridOnDraw(i, grid::CellType::Start);
			m_SavedGridSummary.start.row = i / m_GridColumns;
			m_SavedGridSummary.start.column = i % m_GridColumns;

			break;
		}
		case(interface::EditMode::Finish): {
			if (m_SavedGridSummary.finish.assigned == true)
				break;
			m_Grid[i].type = grid::CellType::Finish;
			UpdateGridOnDraw(i, grid::CellType::Finish);
			m_SavedGridSummary.finish.row = i / m_GridColumns;;
			m_SavedGridSummary.finish.column = i % m_GridColumns;
			break;
		}
		case(interface::EditMode::Erase): {
			if (m_SavedGridSummary.translations.empty() == true)
				break;
			m_Grid[i].type = grid::CellType::None;
			EraseGrid(i);
			UpdateGridOnDraw(i, grid::CellType::None);
			break;
		}
		}
;
	}

	void grid::GridMap::OnWindowResize(glm::ivec2 newDim)
	{
		m_GridDimensions = newDim;


		int add = m_GridDimensions.y % GridSize();
		int size = m_GridDimensions.y + (add > 0 ? GridSize() - add : 0);
		int lineCountHori = size / GridSize();
		float step = (2.f * (size / (float)m_GridDimensions.y)) / lineCountHori;

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

		add = m_GridDimensions.x % GridSize();
		size = m_GridDimensions.x + (add > 0 ? GridSize() - add : 0);
		int lineCountVert = size / GridSize();
		step = (2.f * (size / (float)m_GridDimensions.x)) / lineCountVert;

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

		
		UpdateGridOnResize(lineCountHori, lineCountVert);


		

		delete[] newVertices;
	}

	int grid::GridMap::SolveGrid()
	{
		if (m_SavedGridSummary.start.assigned == false || m_SavedGridSummary.finish.assigned == false)
			return false;

		uint32_t **mat = new uint32_t*[m_GridRows];
		for (uint32_t i = 0; i < m_GridRows; ++i)
			mat[i] = new uint32_t[m_GridColumns];


		std::cout << "\n";
		for (uint32_t i = 0; i < m_GridRows; i++)
		{
			for (uint32_t j = 0; j < m_GridColumns; j++)
			{
				if (GetGridAt(i, j)->type == CellType::Wall)
					mat[i][j] = 1;
				else
					mat[i][j] = 0;
				//std::cout << mat[i][j] << " ";
			}
			//std::cout << "\n";
		}

		

		if (mat[m_SavedGridSummary.start.row][m_SavedGridSummary.start.column] || mat[m_SavedGridSummary.finish.row][m_SavedGridSummary.finish.column])
			return -1;

		bool **visited = new bool*[m_GridRows];
		for (uint32_t i = 0; i < m_GridRows; ++i)
			visited[i] = new bool[m_GridColumns];

		//memset(visited, false, sizeof(bool) * m_GridRows * m_GridRows * m_GridColumns);

		for (uint32_t i = 0; i < m_GridRows; i++)
		{
			for (uint32_t j = 0; j < m_GridColumns; j++)
			{
				visited[i][j] = false;
				//std::cout << visited[i][j] << " ";
			}
			//std::cout << "\n";
		}


		// Mark the source cell as visited 
		visited[m_SavedGridSummary.start.row][m_SavedGridSummary.start.column] = true;

		/*for (uint32_t i = 0; i < m_GridRows; i++)
		{
			for (uint32_t j = 0; j < m_GridColumns; j++)
			{
				std::cout << visited[i][j] << " ";
			}
			std::cout << "\n";
		}*/

		// Create a queue for BFS 
		std::queue<queueNode> q;

		// Distance of source cell is 0 
		Grid src;
		src.column = m_SavedGridSummary.start.column;
		src.row = m_SavedGridSummary.start.row;
		queueNode s = { src, 0 };
		q.push(s);  // Enqueue source cell 
		std::vector<std::pair<uint32_t, uint32_t>> stepPairs;

		// Do a BFS starting from source cell 
		start = SDL_GetTicks();
		while (!q.empty())
		{
			queueNode curr = q.front();
			Grid pt = curr.pt;

			// If we have reached the destination cell, 
			// we are done 
			if (pt.row == m_SavedGridSummary.finish.row && pt.column == m_SavedGridSummary.finish.column)
			{
				start = SDL_GetTicks();

				OnGridSolved(&curr.pairs, &stepPairs);
				
				return curr.dist;
			}

			// Otherwise dequeue the front cell in the queue 
			// and enqueue its adjacent cells 
			q.pop();

			bool available = false;
			std::vector<queueNode> availableNode;
			for (int i = 0; i < (m_Diagonal ? rowNum.size() : rowNum.size() / 2); i++)
			{
				int row = pt.row + rowNum[i];
				int col = pt.column + colNum[i];

				// if adjacent cell is valid, has path and 
				// not visited yet, enqueue it. 
				if (isValid(row, col) && mat[row][col] == 0 &&
					!visited[row][col])
				{
					available = true;
					stepPairs.push_back(std::make_pair(row, col));
					// mark cell as visited and enqueue it 
					visited[row][col] = true;
					//mat[row][col] = 5;
					queueNode Adjcell = { {row, col},
										  curr.dist + 1 };
				
					Adjcell.pairs = curr.pairs;
					Adjcell.pairs.push_back(std::make_pair(row, col));
					q.push(Adjcell);
				}
			}

		}

		// Return -1 if destination cannot be reached 
		return -1;
	}

	void grid::GridMap::SetDiagonal(bool val)
	{
		m_Diagonal = val;
	}

	void grid::GridMap::SetRenderPath(bool val)
	{
		m_RenderPath = val;
	}

	void grid::GridMap::SetRenderAttemps(bool val)
	{
		m_RenderAttemps = val;
	}

	void grid::GridMap::SetShowSteps(bool val)
	{
		m_ShowSteps = val;
	}

	void grid::GridMap::SetRenderSpeed(uint32_t speed)
	{
		uint32_t s = speed;
		/*if (speed > 0)
			s = 5000 / speed;*/
		std::cout << s << std::endl;
		m_SavedGridSummary.pathRenderData.timer.ChangeTargetTime(s);
		m_SavedGridSummary.attemptsRenderData.timer.ChangeTargetTime(s);
	}

	void grid::GridMap::EraseGrid(uint32_t index)
	{
		int row = index / m_GridColumns;
		int column = index % m_GridColumns + 1;

		auto it = std::find_if(m_SavedGridSummary.grid_data.begin(), m_SavedGridSummary.grid_data.end(),
			[&](const grid::SavedGridData & ref) { return ref.row == row && ref.column == column; });
		
		auto itTrans = std::find_if(m_SavedGridSummary.translations_ref.begin(), m_SavedGridSummary.translations_ref.end(),
			[&](const grid::Grid & ref) { return ref.row == row && ref.column == column; });
		
		if (it != m_SavedGridSummary.grid_data.end() &&
			itTrans != m_SavedGridSummary.translations_ref.end())
		{
			if (m_SavedGridSummary.grid_data.at(std::distance(m_SavedGridSummary.grid_data.begin(), it)).type == grid::CellType::Start)
				m_SavedGridSummary.start.assigned = false;
			if (m_SavedGridSummary.grid_data.at(std::distance(m_SavedGridSummary.grid_data.begin(), it)).type == grid::CellType::Finish)
				m_SavedGridSummary.finish.assigned = false;

			m_SavedGridSummary.grid_data.erase(it);

			m_SavedGridSummary.translations.erase(m_SavedGridSummary.translations.begin() + 
												  std::distance(m_SavedGridSummary.translations_ref.begin(), itTrans));
			m_SavedGridSummary.translations_ref.erase(itTrans);
		}
	}

	uint32_t grid::GridMap::GridSize()
	{
		return GRID_SIZE * m_GridScale;
	}

	bool grid::GridMap::isValid(uint32_t row, uint32_t col)
	{
		return (row >= 0) && (row < m_GridRows) &&
			(col >= 0) && (col < m_GridColumns);
	}

	void grid::GridMap::AssignGridData(uint32_t index, grid::CellType cellType, glm::vec3 color)
	{
		float sizendcX = (float)GridSize() / (float)m_GridDimensions.x * 2.f;
		float sizendcY = (float)GridSize() / (float)m_GridDimensions.y * 2.f;
		int row = index / m_GridColumns;
		int column = index % m_GridColumns + 1;

		grid::SavedGridData gridData;
		gridData.row = row;
		gridData.column = column;
		gridData.type = cellType;
		VertexData vertData;
		vertData.translation = glm::vec2(sizendcX * column, sizendcY * row * -1);
		vertData.color = color;
		m_SavedGridSummary.grid_data.push_back(gridData);
		m_SavedGridSummary.translations.push_back(vertData);
		Grid transData;
		transData.row = row;
		transData.column = column;
		m_SavedGridSummary.translations_ref.push_back(transData);
		//std::sort(m_SavedGridSummary.grid_data.begin(), m_SavedGridSummary.grid_data.end(), [&](const grid::SavedGridData& a, const grid::SavedGridData & b) { return a.row < b.row; });
	}

	void grid::GridMap::OnGridSolved(std::vector<std::pair<uint32_t, uint32_t>> *path, std::vector<std::pair<uint32_t, uint32_t>> *attempts)
	{
		// Render attempts
		
		if (m_RenderAttemps == true && m_ShowSteps == true)
		{
			if (m_RenderPath == true)
			{
				/*for (auto & i : *path)
				{
					auto it = std::find_if(attempts->begin(), attempts->end(), [&]
					(const std::pair<uint32_t, uint32_t> & ref) { return ref.first == i.first && ref.second == i.second;  });
					if (it != attempts->end())
						attempts->erase(it);
				}*/
			}

			m_SavedGridSummary.attemptsRenderData = AttemptsRenderData(*attempts);
		}
		else if (m_RenderAttemps == true)
		{
			if (m_RenderPath == true)
			{
				/*for (auto & i : *path)
				{
					auto it = std::find_if(attempts->begin(), attempts->end(), [&]
					(const std::pair<uint32_t, uint32_t> & ref) { return ref.first == i.first && ref.second == i.second;  });
					if (it != attempts->end())
						attempts->erase(it);
				}*/
			}

			for (auto & i : *attempts)
			{
				AssignGridData(i.first * m_GridRows + i.second, CellType::Attempt, GRID_ATTEMPT_COLOR);
			}

			shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());
		}

		// Render path
		if (m_RenderPath == true && m_ShowSteps == true)
		{
			m_SavedGridSummary.pathRenderData = PathRenderData(*path);
		}
		else if(m_RenderPath == true)
		{
			for (auto & i : *path)
			{
				AssignGridData(i.first * m_GridColumns + i.second, CellType::Path, GRID_PATH_COLOR);
			}

			

			shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());
		}
	}

	void grid::GridMap::ClearGrid()
	{
		m_SavedGridSummary.grid_data.clear();
		m_SavedGridSummary.translations.clear();
		m_SavedGridSummary.translations_ref.clear();
		m_SavedGridSummary.start.assigned = false;
		m_SavedGridSummary.finish.assigned = false;
		m_SavedGridSummary.pathRenderData.render = false;
		m_SavedGridSummary.attemptsRenderData.render = false;
		
		for (int i = 0; i < m_GridRows; i++)
		{
			for (int j = 0; j < m_GridColumns; j++)
			{
				m_Grid[i * m_GridColumns + j].type = CellType::None;
			}
		}
		shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());

	}

#ifdef _DEBUG_
	void grid::GridMap::PrintGrid()
	{
		std::cout << "\n\n\n" << "Rows: " << m_GridRows << "\n" << "Columns: " << m_GridColumns << std::endl;
		for (int i = 0; i < m_GridRows; i++)
		{
			for (int j = 0; j < m_GridColumns; j++)
			{
				if(m_Grid[i * m_GridColumns + j].type == CellType::None)
					std::cout << 0 << " ";
				else
					std::cout << 1 << " ";
			}
			std::cout << std::endl;
		}
	}
#endif


}