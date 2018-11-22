#version 330 core
layout (location = 0) in vec2 Pos;
layout (location = 1) in vec2 Offset;
layout (location = 2) in vec3 Color;


out vec3 fColor;
out vec2 aPos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	aPos = Pos;
	fColor = Color;

	gl_Position = vec4(aPos + Offset, 0.0, 1.0);
}
