#shader vertex
#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normal;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	//fragPosition = vec3(model * vec4(position, 1.0));
	//fragPosition = vec3(gl_Position);
	normal = vertexNormal;
}

#shader geometry
#version 440 core

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

in vec3 normal[];

void main()
{
	vec4 pos = gl_in[0].gl_Position;
	gl_Position = pos;
	EmitVertex();
	pos = gl_in[1].gl_Position;
	gl_Position = pos;
	EmitVertex();
	pos = gl_in[2].gl_Position;
	gl_Position = pos;
	EmitVertex();
	pos = gl_in[0].gl_Position;
	gl_Position = pos;
	EmitVertex();
	EndPrimitive();
}

#shader fragment
#version 440 core

layout(location = 0) out vec4 color;

uniform vec3 viewPos;

void main() 
{
	color = vec4(1, 1, 0, 1);
		


	/*if (fragPosition.y == normal.y)
		color = vec4(1, 0, 0, 1);
	if (fragPosition.y > 1 && fragPosition.x > 1)
		color = vec4(0, 1, 0, 1);
	if (fragPosition.y < 0)
		color = vec4(0, 0, 1, 1);*/
	//color = vec4(1, 1, 1, 1);
}