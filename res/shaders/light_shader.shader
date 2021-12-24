#shader vertex
#version 440 core

layout(location = 0) in vec4 position;


uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position);
};


#shader fragment
#version 440 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(1.0f);
};
