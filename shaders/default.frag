#version 330 core

in vec2 aPos;

uniform vec3 color = vec3(0.0, 1.0, 0.0);

void main()
{
	gl_FragColor = vec4(color, 1.0);
}