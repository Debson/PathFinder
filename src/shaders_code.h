#ifndef SHADERS_CODE_H
#define SHADERS_CODE_H

#include <iostream>

namespace md
{
	namespace shaders_code
	{

		const std::string gridVert = "#version 330 core\n\
			layout(location = 0) in vec2 Pos;\n\
\n\
		out vec2 aPos;\n\
\n\
		void main()\n\
		{\n\
			aPos = Pos;\n\
\n\
			gl_Position = vec4(aPos, 0.0, 1.0);\n\
		}\n\
		";

		const std::string defaultVert = "#version 330 core\n\
			layout(location = 0) in vec2 Pos;\n\
		layout(location = 1) in vec2 Offset; \n\
		layout(location = 2) in vec3 Color; \n\
			\n\
			\n\
		out vec3 fColor; \n\
		out vec2 aPos; \n\
			\n\
		uniform mat4 model; \n\
		uniform mat4 projection; \n\
			\n\
		void main()\n\
		{\n\
			aPos = Pos; \n\
			fColor = Color; \n\
			\n\
			gl_Position = vec4(aPos + Offset, 0.0, 1.0); \n\
		}\n\
		";

		const std::string defaultFrag = "#version 330 core\n\
\n\
		in vec2 aPos;\n\
		in vec3 fColor;\n\
\n\
		uniform vec3 vColor = vec3(0.0, 1.0, 0.0);\n\
		uniform bool colorGrid = false;\n\
\n\
		void main()\n\
		{\n\
			vec3 color;\n\
			if (colorGrid)\n\
				color = vColor;\n\
			else\n\
				color = fColor; \n\
			gl_FragColor = vec4(color, 1.0);\n\
		}\n\
		";

		const std::string defaultGeom = "#version 330 core\n\
		layout(points) in;\n\
		layout(line_strip, max_vertices = 4) out;\n\
\n\
		uniform bool vertical = false;\n\
\n\
		void build(vec4 pos)\n\
		{\n\
			if (vertical == false)\n\
			{\n\
				gl_Position = pos;\n\
				EmitVertex();\n\
				gl_Position = pos + vec4(2.0, 0.0, 0.0, 0.0);\n\
				EmitVertex();\n\
			}\n\
			else\n\
			{\n\
				gl_Position = pos;\n\
				EmitVertex();\n\
				gl_Position = pos - vec4(0.0, 2.0, 0.0, 0.0);\n\
				EmitVertex();\n\
			}\n\
			\n\
			EndPrimitive();\n\
		}\n\
\n\
		void main()\n\
		{\n\
			build(gl_in[0].gl_Position);\n\
		}\n\
		";
	}
}

#endif // !SHADERS_CODE_H

