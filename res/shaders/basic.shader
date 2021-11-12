#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

out vec3 fragmentColor;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position);
	fragmentColor = vertexColor;
	normal = vertexNormal;
};


#shader fragment
#version 330 core

in vec3 fragmentColor;
in vec3 normal;

uniform vec3 lightColor;
uniform vec3 lightPos;


layout(location = 0) out vec3 color;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 result = ambient * fragmentColor;



	color = result;

	//color = vec3(1, 0, 0);
};