#shader vertex
#version 440 core

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
	// translate to model normal matrix ???
	normal = mat3(transpose(inverse(model))) * vertexNormal;
	// transform frag positions to world space ??? for lighting
 	fragPosition = vec3(model * vec4(position, 1.0));
};


#shader fragment
#version 440 core

in vec3 normal;
in vec3 fragPosition;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;

layout(location = 0) out vec4 color;

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
	float specularStrength = 0.1;

	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 resultingColor = vec3(1, 0.5, 1);

	color = vec4((ambient + diffuse + specular) * resultingColor, 1);

	//color = vec4(1, 1, 0, 1);
};