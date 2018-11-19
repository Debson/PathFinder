#version 330 core
layout (location = 0) in vec2 Pos;

out vec2 aPos;

void main()
{
	aPos = Pos;

	gl_Position = vec4(aPos, 0.0, 1.0);
}
