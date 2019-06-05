#ifndef SHADER_CONF_H
#define SHADER_CONF_H

#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>


#include "md_shader.h"
#include "grid.h"

namespace md
{
namespace shader
{
	/*const float vertices[] =
	{
		// positions
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};*/

	const float vertices[] = {
		// positions     // colors
		-1.05f,  1.00f, //  top left
		-1.00f,	 0.95f, // bottom right
		-1.05f,  0.95f,	//bottom left

		-1.05f,  1.00f,	// top left
		-1.00f,  0.95f, // bottom right
		-1.00f,  1.00f // top right
	};

	/*const float vertices[]
	{
		 1.f,  1.f,
		 1.f, -1.f,
		-1.f, -1.f,
		-1.f,  1.f
	};

	const unsigned int indices[]
	{
		0, 1, 3,
		1, 2, 3
	};*/

	const float points[] = {
		-1.0f,  0.6f, // top-left
		-1.0f,  0.2f, // top-right
		-1.0f,  -0.2f, // bottom-right
		-1.0f,  -0.6f  // bottom-left
	};
	

	void Start(glm::ivec2 dim);

	static mdShader *GetShader();
	glm::mat4 *GetProjection();
	GLuint *GetVAO();
	GLuint *GetVBO();
	void UpdateProjectionMatrix(glm::ivec2 dim);
	void UpdateGridHorizontal(float *newPoints, unsigned int lineCount);
	void UpdateGridVertical(float *newPoints, unsigned int lineCount);
	void UpdateShader(grid::VertexData *data, unsigned int drawCalls, float *newVertices);
	void UpdateOnCollision(grid::VertexData *data, unsigned int drawCalls);
	void Draw();
	void Draw(glm::vec2 pos, glm::vec2 size, bool drawUnderGrid = false);
	void DrawGrid(bool vertical = false);

}
}

#endif // !SHADER_CONF_H
