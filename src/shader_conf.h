#ifndef SHADER_CONF_H
#define SHADER_CONF_H

#include <glm.hpp>
#include <GL/gl3w.h>

#include "md_shader.h"

namespace md
{
namespace shader
{
	const float vertices[] =
	{
		// positions
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

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
	void Draw();
	void Draw(glm::vec2 pos, glm::vec2 size, bool drawUnderGrid = false);
	void DrawGrid(bool vertical = false);

}
}

#endif // !SHADER_CONF_H
