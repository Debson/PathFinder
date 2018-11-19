#version 330 core
layout (location = 0) in vec2 Pos;

out vec2 aPos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	aPos = Pos;

	gl_Position = projection * model * vec4(aPos, 1.0, 1.0);
}
