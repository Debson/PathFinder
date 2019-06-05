#include "grid.h"

#include <queue>
#include <algorithm>
#include <set>

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
			std::vector<Grid> pairs;

		};

		//std::vector<int> rowNum = { -1,  0, 0, 1, -1,  1, 1, -1 };
		//std::vector<int> colNum = {  0, -1, 1, 0, -1, -1, 1,  1 };

        std::vector<int> rowNum = { -1, 1, 0,  0, -1, -1, 1,  1 };
        std::vector<int> colNum = {  0, 0, 1, -1,  1, -1, 1, -1 };

        //std::vector<int> rowNum = { -1, -1, 1,  1, -1,  1, 0,  0 };
        //std::vector<int> colNum = {  1, -1, 1, -1,  0,  0, 1, -1 };


        typedef std::pair<int, int> GridPair;
	}

	int grid::GridMap::m_GridRows			= 0;
    grid::SavedGridSummary grid::GridMap::m_SavedGridSummary;
	int grid::GridMap::m_GridColumns		= 0;
	glm::ivec2 grid::GridMap::m_GridDimensions;
	float grid::GridMap::m_GridScale			= 1.f;
	bool grid::GridMap::m_Diagonal				= true;
	bool grid::GridMap::m_RenderPath			= true;
	bool grid::GridMap::m_RenderAttemps			= true;
	bool grid::GridMap::m_ShowSteps				= false;
	bool grid::GridMap::m_Active				= false;
	int grid::GridMap::m_RenderSpeed			= 100;
	int grid::GridMap::m_CalculationsTime = 0;
	const char *grid::GridMap::m_CurrentAlgorithm = A_STAR_ALGORITHM;

	grid::GridMap::GridMap() { }

	grid::GridMap::~GridMap()
	{
		
	}

	void grid::GridMap::Start()
	{
		
	}

	void grid::GridMap::Update()
	{
		if(input::IsKeyDown(input::KeyCode::MouseLeft))
			m_Active = true;
	}

	void grid::GridMap::Render()
	{
		// Draw path if registered
		if(m_SavedGridSummary.pathRenderData.render == true && m_SavedGridSummary.pathRenderData.timer.isFinished() == true && m_SavedGridSummary.attemptsRenderData.render == false)
		{ 
			for (int i = 0; i < m_SavedGridSummary.pathRenderData.step; i++)
			{
				auto & pair = m_SavedGridSummary.pathRenderData.pairs[m_SavedGridSummary.pathRenderData.counter];
			    EraseGrid(pair.row, pair.column);
				UpdateGridOnDraw(pair.row * m_GridColumns + pair.column, grid::CellType::Path);
				m_SavedGridSummary.pathRenderData.counter++;
				m_SavedGridSummary.pathRenderData.timer.Restart();
				if (m_SavedGridSummary.pathRenderData.counter > m_SavedGridSummary.pathRenderData.pairs.size() - 1)
				{
					m_SavedGridSummary.pathRenderData.render = false;
					break;
				}
			}
		}

		// Draw attemps if registered
		if (m_SavedGridSummary.attemptsRenderData.render == true && m_SavedGridSummary.attemptsRenderData.timer.isFinished() == true)
		{
			for (int i = 0; i < m_SavedGridSummary.attemptsRenderData.step; i++)
			{
				auto & pair = m_SavedGridSummary.attemptsRenderData.pairs[m_SavedGridSummary.attemptsRenderData.counter];
				UpdateGridOnDraw(pair.row * m_GridColumns + pair.column, grid::CellType::Attempt);
				m_SavedGridSummary.attemptsRenderData.counter++;
				m_SavedGridSummary.attemptsRenderData.timer.Restart();
				if (m_SavedGridSummary.attemptsRenderData.counter > m_SavedGridSummary.attemptsRenderData.pairs.size() - 1)
				{
					m_SavedGridSummary.attemptsRenderData.render = false;

					break;
				}
			}
		}

		// Draw colored squares
		shader::Draw();
	}

	void grid::GridMap::UpdateGridOnResize(uint16_t rows, uint16_t columns)
	{
		m_GridRows = rows;
		m_GridColumns = columns;

		float sizendcX = (float)GridSize() / (float)m_GridDimensions.x * 2.f;
		float sizendcY = (float)GridSize() / (float)m_GridDimensions.y * 2.f;

		m_SavedGridSummary.translations.clear();
		m_SavedGridSummary.translations_ref.clear();


		for(auto & i : m_SavedGridSummary.grid_data)
		{
			if (i.row < rows && i.column < columns)
			{

				VertexData vertData;
				vertData.translation = glm::vec2(sizendcX * (i.column + 1), sizendcY * i.row * -1);
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
                    // Prevent from a bug when parts of the paths are diagonal and grid is resized
                    bool shouldContinue = false;
                    for(auto & g : m_SavedGridSummary.pathRenderData.pairs)
                    {
                        if(g.row == i.row && g.column == i.column)
                        {
                            shouldContinue = true;
                            break;
                        }
                    }

                    if(shouldContinue)
                        continue;
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

	void grid::GridMap::UpdateGridOnDraw(int index, grid::CellType cellType)
	{
		switch (cellType)
		{
		case(CellType::None): {
				break;
			}
		case(CellType::Wall): {
			int row = index / m_GridColumns;
			int column = index % m_GridColumns;

			// check if already exists
			auto it = std::find_if(m_SavedGridSummary.grid_data.begin(), m_SavedGridSummary.grid_data.end(),
				[&](const grid::SavedGridData & ref) { return ref.row == row && ref.column == column; });
			if (it != m_SavedGridSummary.grid_data.end())
			{
			    // Prevents from drawing multiple wall cells at the same coordinates or at Start or Finish fields
			    if((*it).type == cellType || (*it).type == CellType::Start || (*it).type == CellType::Finish)
			        return;

			    EraseGrid(row, column);
			}

			AssignGridData(index, cellType, GRID_WALL_COLOR);
			break;
		}
		case(CellType::Start): {
		    if(!m_SavedGridSummary.start.assigned)
		    {
                AssignGridData(index, cellType, GRID_START_COLOR);
                m_SavedGridSummary.start.assigned = true;
                m_SavedGridSummary.start.row = index / m_GridColumns;
                m_SavedGridSummary.start.column = index % m_GridColumns;
            }
			break;
		}
		case(CellType::Finish): {
		    if(!m_SavedGridSummary.finish.assigned)
		    {
                AssignGridData(index, cellType, GRID_FINISH_COLOR);
                m_SavedGridSummary.finish.assigned = true;
                m_SavedGridSummary.finish.row = index / m_GridColumns;;
                m_SavedGridSummary.finish.column = index % m_GridColumns;
            }
			break;
		}
		case(CellType::Attempt): {
			
			AssignGridData(index, cellType, GRID_ATTEMPT_COLOR);
			
			break;
		}
		case(CellType::Path): {
			int row = index / m_GridColumns;
			int column = index % m_GridColumns;
			auto it = std::find_if(m_SavedGridSummary.translations_ref.begin(), m_SavedGridSummary.translations_ref.end(),
				[&](const grid::Grid & ref) { return ref.row == row && ref.column == column; });
			if (it != m_SavedGridSummary.translations_ref.end())
			{
				m_SavedGridSummary.translations.erase(m_SavedGridSummary.translations.begin() + std::distance(m_SavedGridSummary.translations_ref.begin(), it));
				m_SavedGridSummary.translations_ref.erase(it);
			}
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
		int i = x / GridSize();
		i += y / GridSize() * m_GridColumns;

		int row = y / GridSize();
		int column = x / GridSize();

		/*CellType cellType = (CellType)m_SavedGridSummary.grid_rep.at(i);
		if ((cellType != CellType::None) && interface::GetEditState() != interface::EditMode::Erase)
			return;*/
		
		switch (interface::GetEditState())
		{
		case(interface::EditMode::Wall): {
			UpdateGridOnDraw(i, grid::CellType::Wall);
			break;
		}
		case(interface::EditMode::Start): {
			if (m_SavedGridSummary.start.assigned == true)
                EraseGrid(m_SavedGridSummary.start.row, m_SavedGridSummary.start.column);

			UpdateGridOnDraw(i, grid::CellType::Start);
			break;
		}
		case(interface::EditMode::Finish): {
			if (m_SavedGridSummary.finish.assigned == true)
                EraseGrid(m_SavedGridSummary.finish.row, m_SavedGridSummary.finish.column);

            UpdateGridOnDraw(i, grid::CellType::Finish);
			break;
		}
		case(interface::EditMode::Erase): {
			if (m_SavedGridSummary.translations.empty() == true)
				break;

			EraseGrid(row, column);
			UpdateGridOnDraw(i, grid::CellType::None);
			break;
		}
		}
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
        if (m_SavedGridSummary.start.row > m_GridRows || m_SavedGridSummary.start.column > m_GridColumns)
        {
            EraseGrid(m_SavedGridSummary.start.row, m_SavedGridSummary.finish.column);
            shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());
        }
        if(m_SavedGridSummary.finish.row > m_GridRows || m_SavedGridSummary.finish.column > m_GridColumns)
        {
            EraseGrid(m_SavedGridSummary.finish.row, m_SavedGridSummary.finish.column);
            shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());
        }

		if (m_SavedGridSummary.start.assigned == false || m_SavedGridSummary.finish.assigned == false)
			return -2;

		int mat[m_GridRows][m_GridColumns];
		memset(mat, 0, sizeof(mat));

		for (auto & i : m_SavedGridSummary.grid_data)
		{
			if(i.type == CellType::Wall)
				mat[i.row][i.column] = 1;
		}


		if (mat[m_SavedGridSummary.start.row][m_SavedGridSummary.start.column] || mat[m_SavedGridSummary.finish.row][m_SavedGridSummary.finish.column])
			return -1;

		bool visited[m_GridRows][m_GridColumns];

		memset(visited, false, sizeof(visited));

        /*  @param  Mat two-dimensional array represent actual grid that needs to be solved
         *  @param  visited two-dimensional array represents cells that have been visited
         *          while finding correct path
         *
         */

		// Mark the source cell as visited
		visited[m_SavedGridSummary.start.row][m_SavedGridSummary.start.column] = true;
		std::vector<Grid> stepPairs;

		m_CalculationsTime = SDL_GetTicks();


        if(strcmp(m_CurrentAlgorithm, A_STAR_ALGORITHM) == 0 || strcmp(m_CurrentAlgorithm, DIJKSTRA_ALGORITHM) == 0) {
            GridAStar gridDetails[m_GridRows][m_GridColumns];

            for (auto &gridRow : gridDetails) {
                for (auto &gridColumn : gridRow) {
                    // h = f + g
                    gridColumn.overall_cost = FLT_MAX;
                    gridColumn.cost_to_finish = FLT_MAX;
                    gridColumn.cost_to_start = FLT_MAX;
                    // Initialize parents
                    gridColumn.row = -1;
                    gridColumn.column = -1;
                }
            }

            int row_index = m_SavedGridSummary.start.row;
            int column_index = m_SavedGridSummary.start.column;

            gridDetails[row_index][column_index].overall_cost = 0.0;
            gridDetails[row_index][column_index].cost_to_start = 0.0;
            gridDetails[row_index][column_index].cost_to_finish = 0.0;
            gridDetails[row_index][column_index].row = row_index;
            gridDetails[row_index][column_index].column = column_index;

            std::vector<std::pair<double, GridPair>> openListDijkstra;
            openListDijkstra = { { 0.0, { row_index, column_index } } };

            std::set<std::pair<double, GridPair>> openList;
            openList = { { 0.0, { row_index, column_index } } };

            while (!openList.empty()) {

                std::pair<double, std::pair<int, int>> gd;

                if(strcmp(m_CurrentAlgorithm, DIJKSTRA_ALGORITHM) == 0)
                {
                    gd = openListDijkstra.front();
                    openListDijkstra.erase(openListDijkstra.begin());
                }
                else
                {
                    gd = *openList.begin();
                    openList.erase(openList.begin());
                }

                row_index = gd.second.first;
                column_index = gd.second.second;
                visited[row_index][column_index] = true;

                double overall_cost_New, cost_to_start_New, cost_to_finish_New;

                for (int i = 0; i < (m_Diagonal ? colNum.size() : colNum.size() / 2.f); i++) {
                    int row_successor = row_index + rowNum[i];
                    int column_successor = column_index + colNum[i];

                    if (isValid(row_successor, column_successor)) {
                        // Destination found
                        if (row_successor == m_SavedGridSummary.finish.row &&
                            column_successor == m_SavedGridSummary.finish.column) {
                            m_CalculationsTime = SDL_GetTicks() - m_CalculationsTime;

                            gridDetails[row_successor][column_successor].row = row_index;
                            gridDetails[row_successor][column_successor].column = column_index;

                            GridAStar *grid[m_GridRows * m_GridColumns];
                            for (int i = 0; i < m_GridRows; i++) {
                                for (int j = 0; j < m_GridColumns; j++) {
                                    grid[i * m_GridColumns + j] = &gridDetails[i][j];
                                }
                            }

                            std::vector<Grid> path;

                            TracePath(*grid, &path);

                            OnGridSolved(&path, &stepPairs);

                            return path.size();
                        } else if (visited[row_successor][column_successor] == false &&
                                   mat[row_successor][column_successor] == 0) {


                            if(strcmp(m_CurrentAlgorithm, DIJKSTRA_ALGORITHM) == 0)
                                overall_cost_New = 0;
                            else
                            {
                                cost_to_start_New = gridDetails[row_index][column_index].cost_to_start + 1.0;
                                cost_to_finish_New = grid::GridMap::CalculateHeuristics(row_successor, column_successor);
                                overall_cost_New = cost_to_start_New + cost_to_finish_New;
                            }


                            if (gridDetails[row_successor][column_successor].overall_cost == FLT_MAX ||
                                gridDetails[row_successor][column_successor].overall_cost > overall_cost_New)
                            {
                                stepPairs.push_back(Grid(row_successor, column_successor));

                                if(strcmp(m_CurrentAlgorithm, DIJKSTRA_ALGORITHM) == 0)
                                    openListDijkstra.push_back(std::make_pair(overall_cost_New, std::make_pair(row_successor, column_successor)));
                                else
                                    openList.insert(std::make_pair(overall_cost_New, std::make_pair(row_successor, column_successor)));

                                gridDetails[row_successor][column_successor].cost_to_start = cost_to_start_New;
                                gridDetails[row_successor][column_successor].cost_to_finish = cost_to_finish_New;
                                gridDetails[row_successor][column_successor].overall_cost = overall_cost_New;
                                gridDetails[row_successor][column_successor].row = row_index;
                                gridDetails[row_successor][column_successor].column = column_index;
                            }
                        }
                    }
                }
            }
        }
        else if(strcmp(m_CurrentAlgorithm, BREADTH_FIRST_SEARCH) == 0)
        {
            // Create a queue for BFS
            std::queue<queueNode> q;

            // Distance of source cell is 0
            Grid src;
            src.column = m_SavedGridSummary.start.column;
            src.row = m_SavedGridSummary.start.row;
            queueNode s = {src, 0};
            q.push(s);  // Enqueue source cell

            // Do a BFS starting from source cell
            while (!q.empty()) {
                queueNode curr = q.front();
                Grid pt = curr.pt;

                // If we have reached the destination cell,
                // we are done
                if (pt.row == m_SavedGridSummary.finish.row && pt.column == m_SavedGridSummary.finish.column) {
                    m_CalculationsTime = SDL_GetTicks() - m_CalculationsTime;

                    curr.pairs.pop_back();
                    std::pair<int, int> start(m_SavedGridSummary.start.row, m_SavedGridSummary.start.column);
                    std::pair<int, int> finish(m_SavedGridSummary.finish.row, m_SavedGridSummary.finish.column);
                    auto it = std::find_if(stepPairs.begin(), stepPairs.end(), [&]
                            (const Grid &ref) { return ref.row == start.first && ref.column == start.second; });
                    if (it != stepPairs.end())
                        stepPairs.erase(it);
                    it = std::find_if(stepPairs.begin(), stepPairs.end(), [&]
                            (const Grid &ref) { return ref.row == finish.first && ref.column == finish.second; });
                    if (it != stepPairs.end())
                        stepPairs.erase(it);


                    OnGridSolved(&curr.pairs, &stepPairs);

                    return curr.dist;
                }

                // Otherwise dequeue the front cell in the queue
                // and enqueue its adjacent cells
                q.pop();

                bool available = false;
                std::vector<queueNode> availableNode;
                for (int i = 0; i < (m_Diagonal ? rowNum.size() : rowNum.size() / 2); i++) {
                    int row = pt.row + rowNum[i];
                    int col = pt.column + colNum[i];

                    // if adjacent cell is valid, has path and
                    // not visited yet, enqueue it.
                    if (isValid(row, col) && mat[row][col] == 0 &&
                        !visited[row][col]) {
                        available = true;
                        stepPairs.push_back(Grid(row, col));
                        // mark cell as visited and enqueue it
                        visited[row][col] = true;
                        //mat[row][col] = 5;
                        queueNode Adjcell = {{row, col},
                                             curr.dist + 1};

                        Adjcell.pairs = curr.pairs;
                        Adjcell.pairs.push_back(Grid(row, col));
                        q.push(Adjcell);
                    }
                }

            }
        }

		m_CalculationsTime = 0;
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

	void grid::GridMap::SetRenderAttempts(bool val)
	{
		m_RenderAttemps = val;
	}

	void grid::GridMap::SetShowSteps(bool val)
	{
		m_ShowSteps = val;
	}

	void grid::GridMap::SetRenderSpeed(int speed)
	{
		int s = abs(100 - speed);
		/*if (speed > 0)
			s = 5000 / speed;*/
		m_SavedGridSummary.pathRenderData.timer.ChangeTargetTime(s);
		m_SavedGridSummary.attemptsRenderData.timer.ChangeTargetTime(s);
	}

	void grid::GridMap::SetAlgorithm(const char *algoName)
	{
	    m_CurrentAlgorithm = algoName;
	}

    void grid::GridMap::EraseGrid(int row, int column)
    {
	    if(m_SavedGridSummary.start.row == row && m_SavedGridSummary.start.column == column)
	        m_SavedGridSummary.start.assigned = false;
	    if(m_SavedGridSummary.finish.row == row && m_SavedGridSummary.finish.column == column)
	        m_SavedGridSummary.finish.assigned = false;

	    for(int i = 0; i < m_SavedGridSummary.grid_data.size(); i++)
        {
	        auto & grid = m_SavedGridSummary.grid_data[i];
	        if(grid.row == row && grid.column == column)
            {
                auto it = m_SavedGridSummary.grid_data.begin() + i;
                *it = std::move(m_SavedGridSummary.grid_data.back());
                m_SavedGridSummary.grid_data.pop_back();

                //break;
                i--;
            }
        }

	    for(int i = 0; i < m_SavedGridSummary.translations_ref.size(); i++)
        {
	        auto & trans = m_SavedGridSummary.translations_ref[i];
	        if(trans.row == row && trans.column == column)
            {
	            auto it = m_SavedGridSummary.translations_ref.begin() + i;
	            *it = std::move(m_SavedGridSummary.translations_ref.back());
	            m_SavedGridSummary.translations_ref.pop_back();

	            auto it_t = m_SavedGridSummary.translations.begin() + i;
	            *it_t = std::move(m_SavedGridSummary.translations.back());
	            m_SavedGridSummary.translations.pop_back();
	            //break;
	            i--;
            }
        }


    }

	int grid::GridMap::GridSize()
	{
		return GRID_SIZE * m_GridScale;
	}

	bool grid::GridMap::isValid(int row, int col)
	{
		return (row >= 0) && (row < m_GridRows) &&
			(col >= 0) && (col < m_GridColumns);
	}

	void grid::GridMap::AssignGridData(int index, grid::CellType cellType, glm::vec3 color)
	{
		float sizendcX = (float)GridSize() / (float)m_GridDimensions.x * 2.f;
		float sizendcY = (float)GridSize() / (float)m_GridDimensions.y * 2.f;
		int row = index / m_GridColumns;
		int column = index % m_GridColumns;


		//*GetGridAt(row, column) = (int)cellType;
		grid::SavedGridData gridData;
		gridData.row = row;
		gridData.column = column;
		gridData.type = cellType;
		VertexData vertData;
		vertData.translation = glm::vec2(sizendcX * (column + 1), sizendcY * row * -1);
		vertData.color = color;
		m_SavedGridSummary.grid_data.push_back(gridData);
		m_SavedGridSummary.translations.push_back(vertData);
		Grid transData;
		transData.row = row;
		transData.column = column;
		m_SavedGridSummary.translations_ref.push_back(transData);
		//std::sort(m_SavedGridSummary.grid_data.begin(), m_SavedGridSummary.grid_data.end(), [&](const grid::SavedGridData& a, const grid::SavedGridData & b) { return a.row < b.row; });
	}

	void grid::GridMap::OnGridSolved(std::vector<Grid> *path, std::vector<Grid> *attempts)
	{
		// Render attempts
		
		if (m_RenderAttemps == true && m_ShowSteps == true)
		{

			m_SavedGridSummary.attemptsRenderData = AttemptsRenderData(*attempts);
		}
		else if (m_RenderAttemps == true)
		{
			if (m_RenderPath == true)
			{
				for (auto & i : *path)
				{
					auto it = std::find_if(attempts->begin(), attempts->end(), [&]
					(const Grid & ref) { return ref.row == i.row && ref.column == i.column;  });
					if (it != attempts->end())
						attempts->erase(it);
				}
			}


			for (auto & i : *attempts)
			{
				AssignGridData(i.row * m_GridColumns + i.column, CellType::Attempt, GRID_ATTEMPT_COLOR);
			}

			m_SavedGridSummary.attemptsRenderData.pairs = *attempts;
		}

        for (auto & i : *path)
        {
            EraseGrid(i.row, i.column);
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
                AssignGridData(i.row * m_GridColumns + i.column, CellType::Path, GRID_PATH_COLOR);
			}

			m_SavedGridSummary.pathRenderData.pairs = *path;
		}

		shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());
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

		
		shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());
	}

	void grid::GridMap::ClearGridPathAndAttempts()
	{
        m_SavedGridSummary.translations.clear();
        m_SavedGridSummary.translations_ref.clear();

        for(int i = 0; i < m_SavedGridSummary.grid_data.size(); i++)
        {
            auto & grid = m_SavedGridSummary.grid_data[i];
            if(grid.type != CellType::Wall && grid.type != CellType::Start && grid.type != CellType::Finish)
            {
                // Most efficient way of removing element from vector
                auto it = m_SavedGridSummary.grid_data.begin() + i;

                *it = std::move(m_SavedGridSummary.grid_data.back());
                m_SavedGridSummary.grid_data.pop_back();

                i--;
            }

        }
        //m_SavedGridSummary.start.assigned = false;
        //m_SavedGridSummary.finish.assigned = false;
        m_SavedGridSummary.pathRenderData.render = false;
        m_SavedGridSummary.attemptsRenderData.render = false;
        UpdateGridOnResize(m_GridRows, m_GridColumns);

		//shader::UpdateOnCollision(m_SavedGridSummary.translations.data(), m_SavedGridSummary.translations.size());
	}

	double grid::GridMap::CalculateHeuristics(int row, int column)
	{
	    /*if(!m_Diagonal)
	        return abs(row - m_SavedGridSummary.finish.row) + abs(column - m_SavedGridSummary.finish.column);*/

	    // Dont need to find a square root, it won't make any difference in that implementation
	    return double(((row - m_SavedGridSummary.finish.row) * (row - m_SavedGridSummary.finish.row) +
	    (column - m_SavedGridSummary.finish.column) * (column - m_SavedGridSummary.finish.column)));
	}

    void grid::GridMap::TracePath(GridAStar grid[], std::vector<Grid> *path)
    {
        int row = m_SavedGridSummary.finish.row;
        int column = m_SavedGridSummary.finish.column;

        while(!(grid[row * m_GridColumns + column].row == row && grid[row * m_GridColumns + column].column == column))
        {
            path->insert(path->begin(), Grid(row, column));
            int temp_row = grid[row * m_GridColumns + column].row;
            int temp_column = grid[row * m_GridColumns + column].column;
            assert(abs(row - temp_row) <= 1 && abs(column - temp_column) <= 1);
            row = temp_row;
            column = temp_column;

         }

        // Erase field that is a finish field
        path->erase(path->end());

        return;
    }

	bool grid::GridMap::IsActive()
	{
		return m_Active;
	}

	bool grid::GridMap::IsRendering()
	{
		return m_SavedGridSummary.pathRenderData.render || m_SavedGridSummary.pathRenderData.render;
	}

	void grid::GridMap::SetActive(bool val)
	{
		m_Active = val;
	}
	
	int grid::GridMap::GetCalculationsTime()
	{
		return m_CalculationsTime;
	}

#ifdef _DEBUG_
	void grid::GridMap::PrintGrid()
	{
		std::cout << "\n\n\n" << "Rows: " << m_GridRows << "\n" << "Columns: " << m_GridColumns << std::endl;
		for (int i = 0; i < m_GridRows; i++)
		{
			for (int j = 0; j < m_GridColumns; j++)
			{
				std::cout << *GetGridAt(i, j) << " ";
			}
			std::cout << std::endl;
		}
	}

	int grid::GridMap::GetObjectsCount()
	{
		return m_SavedGridSummary.grid_data.size();
	}
#endif


}