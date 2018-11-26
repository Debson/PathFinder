#ifndef MD_SHADER_H
#define MD_SHADER_H

#include <GL/gl3w.h>
#include <glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class mdShader
{
public:
	GLuint ID;

	mdShader();
	mdShader(std::string vertexCode, std::string fragmentCode, std::string geometryCode = std::string());

	static mdShader* VertFrag(const GLchar* vertexPath, const GLchar* fragmentPath);
	static mdShader* VertFragGeo(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* gemoetryPath);

	void use();
	void setBool(const std::string &name, GLboolean value) const;
	void setInt(const std::string &name, GLint value) const;
	void setFloat(const std::string &name, GLfloat value) const;
	void setVec2(const std::string &name, glm::vec2 &value) const;
	void setVec2(const std::string &name, GLfloat x, GLfloat y) const;
	void setVec3(const std::string &name, glm::vec3 value) const;
	void setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const;
	void setVec4(const std::string &name, glm::vec4 &value) const;
	void setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
	void setMat2(const std::string &name, glm::mat2 &mat) const;
	void setMat3(const std::string &name, glm::mat3 &mat) const;
	void setMat4(const std::string &name, glm::mat4 &mat) const;


private:
	void checkCompileErrors(GLuint shader, std::string type);

};


#endif // !MD_SHADER_H