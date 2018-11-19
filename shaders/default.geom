#version 330 core
layout (points) in;
layout (line_strip, max_vertices=4) out;

uniform bool vertical = false;

void build(vec4 pos)
{
	if(vertical == false)
	{
		gl_Position = pos;
		EmitVertex();   
		gl_Position = pos + vec4(2.0, 0.0, 0.0, 0.0);
		EmitVertex(); 
	}
	else
	{
		gl_Position = pos;
		EmitVertex();   
		gl_Position = pos - vec4(0.0, 2.0, 0.0, 0.0);
		EmitVertex(); 	
	}


    EndPrimitive();
}


void main()
{
	build(gl_in[0].gl_Position);
}