#include "shader_conf.h"

#include <gtc/matrix_transform.hpp>

namespace md
{
	namespace shader
	{
		mdShader *m_Shader;
		mdShader *m_GridShaderHorizontal;
		mdShader *m_GridShaderVertical;
		glm::mat4 m_Projection;
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;
		GLuint m_InstanceVBO;
		GLuint m_GridHorizontalVAO;
		GLuint m_GridHorizontalVBO;
		GLuint m_GridVerticalVAO;
		GLuint m_GridVerticalVBO;
		

		unsigned int m_LineCounterHori = 4;
		unsigned int m_LineCounterVert = 4;
		unsigned int m_DrawCalls = 2;
	}

	void shader::Start(glm::ivec2 dim)
	{
		// do error checking...
		m_Shader = new mdShader("shaders//default.vert", "shaders//default.frag");

		glGenBuffers(1, &m_InstanceVBO);
		glGenBuffers(1, &m_VBO);
		//glGenBuffers(1, &m_EBO);
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO); 
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1);

		glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		m_Projection = glm::ortho(0.f, static_cast<float>(dim.x), static_cast<float>(dim.y), 0.f);
		m_Shader->use();
		m_Shader->setMat4("projection", m_Projection);


		m_GridShaderHorizontal = new mdShader("shaders//grid.vert", "shaders//default.frag", "shaders//default.geom");

		glGenBuffers(1, &m_GridHorizontalVBO);
		glGenVertexArrays(1, &m_GridHorizontalVAO);
		glBindVertexArray(m_GridHorizontalVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_GridHorizontalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		m_GridShaderHorizontal->use();
		m_GridShaderHorizontal->setMat4("projection", m_Projection);


		m_GridShaderVertical = new mdShader("shaders//grid.vert", "shaders//default.frag", "shaders//default.geom");

		glGenBuffers(1, &m_GridVerticalVBO);
		glGenVertexArrays(1, &m_GridVerticalVAO);
		glBindVertexArray(m_GridVerticalVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_GridVerticalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		m_GridShaderVertical->use();
		m_GridShaderVertical->setMat4("projection", m_Projection);

	}

	mdShader *shader::GetShader()
	{
		return m_Shader;
	}

	glm::mat4 *shader::GetProjection()
	{
		return &m_Projection;
	}

	GLuint *shader::GetVAO()
	{
		return &m_VAO;
	}

	GLuint *shader::GetVBO()
	{
		return &m_VBO;
	}

	void shader::UpdateProjectionMatrix(glm::ivec2 dim)
	{
		m_Projection = glm::ortho(0.f, static_cast<float>(dim.x), static_cast<float>(dim.y), 0.f);
		m_Shader->use();
		m_Shader->setMat4("projection", m_Projection);

		m_Projection = glm::ortho(0.f, static_cast<float>(dim.x), static_cast<float>(dim.y), 0.f);
		m_GridShaderHorizontal->use();
		m_GridShaderHorizontal->setMat4("projection", m_Projection);
	}

	void shader::UpdateGridHorizontal(float *newPoints, unsigned int lineCount)
	{
		m_LineCounterHori = lineCount;
		unsigned int size = lineCount * 2;
		glBindBuffer(GL_ARRAY_BUFFER, m_GridHorizontalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(newPoints) * size, newPoints, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void shader::UpdateGridVertical(float *newPoints, unsigned int lineCount)
	{
		m_LineCounterVert = lineCount;
		unsigned int size = lineCount * 2;
		glBindBuffer(GL_ARRAY_BUFFER, m_GridVerticalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(newPoints) * size, newPoints, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void shader::UpdateShader(grid::VertexData *data, unsigned int drawCalls, float *newVertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(newVertices) * 12, newVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_DrawCalls = drawCalls;
		glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(grid::VertexData) * drawCalls, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void shader::UpdateOnCollision(grid::VertexData *data, unsigned int drawCalls)
	{
		m_DrawCalls = drawCalls;
		glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(grid::VertexData) * drawCalls, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);;
	}

	void shader::Draw()
	{
		m_Shader->use();
		glBindVertexArray(m_VAO);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_DrawCalls);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);;
	}

	void shader::Draw(glm::vec2 pos, glm::vec2 size, bool drawUnderGrid)
	{
		glm::mat4 model;
		m_Shader->use();
		model = glm::translate(model, glm::vec3(pos, drawUnderGrid ? -0.1f : 1.f));
		model = glm::scale(model, glm::vec3(size, drawUnderGrid ? -0.1f : 1.f));
		m_Shader->setMat4("model", model);
		shader::Draw();
	}

	void shader::DrawGrid(bool vertical)
	{
		m_GridShaderVertical->use();
		m_GridShaderVertical->setBool("vertical", vertical);
		if (vertical == false)
		{
			m_GridShaderHorizontal->use();
			m_GridShaderHorizontal->setBool("vertical", vertical);
			m_GridShaderHorizontal->setBool("colorGrid", true);
			m_GridShaderHorizontal->setVec3("vColor", GRID_LINE_COLOR);
			glBindVertexArray(m_GridHorizontalVAO);
			glDrawArrays(GL_POINTS, 0, m_LineCounterHori);
		}
		else
		{
			m_GridShaderVertical->use();
			m_GridShaderVertical->setBool("vertical", vertical);
			m_GridShaderHorizontal->setBool("colorGrid", true);
			m_GridShaderHorizontal->setVec3("vColor", GRID_LINE_COLOR);
			glBindVertexArray(m_GridVerticalVAO);
			glDrawArrays(GL_POINTS, 0, m_LineCounterVert);
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}