#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <glm.hpp>
#define _DEBUG_

#define GRID_SIZE 30
#define GRID_START_SCALE 1.00
#define GRID_SCALE_STEP 0.05
#define GRID_MIN_SCALE 0.2
#define GRID_MAX_SCALE 2.00

const glm::vec3 GRID_COLOR(0.9f, 0.9f, 0.9f);
const glm::vec3 GRID_LINE_COLOR(0.3f, 0.3f, 0.3f);
const glm::vec3 GRID_START_COLOR(0.f, 1.f, 0.f);
const glm::vec3 GRID_FINISH_COLOR(0.f, 0.f, 1.f);
const glm::vec3 GRID_WALL_COLOR(0.f, 0.f, 0.f);
const glm::vec3 GRID_STEP_COLOR(0.5f, 1.f, 1.f);
const glm::vec3 GRID_PATH_COLOR(0.f, 1.f, 0.5f);
#endif // !CONFIGURATION_H