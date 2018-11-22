#version 330 core

in vec2 aPos;
in vec3 fColor;


uniform vec3 vColor = vec3(0.0, 1.0, 0.0);
uniform bool colorGrid = false;

void main()
{
	vec3 color;
	if(colorGrid)
		color = vColor;
	else
		color = fColor;
	gl_FragColor = vec4(color, 1.0);
}