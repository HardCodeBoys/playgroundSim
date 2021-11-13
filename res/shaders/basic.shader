#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexNormal;

out vec3 normal;
out vec3 fragPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	normal = mat3(transpose(inverse(model))) * vertexNormal;
	fragPosition = vec3(model * vec4(position, 1.0));
};


#shader fragment
#version 330 core

in vec3 normal;
in vec3 fragPosition;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;

layout(location = 0) out vec3 color;

void main()
{
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;


	// diffuse lighting

	vec3 lightDir = normalize(lightPos - fragPosition);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular lighting
	float specularStrength = 0.5;

	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	color = (ambient + diffuse + specular) * vec3(1, 1, 0);

	//color = vec3(1, 1, 0);
};