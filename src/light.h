#pragma once
#include "GLHelper.h"

class Light {
private:
	std::vector<glm::vec3> vertices;
	glm::vec3 position;
	GLuint shaderID;
	GLuint vertexBuffer;
public:
	Light(const glm::vec3& _position, GLuint shader) :
		position(_position), shaderID(shader)
	{
		vertices.push_back(position - glm::vec3(-1, 0, 0));
		vertices.push_back(position - glm::vec3(1, 0, 0));
		vertices.push_back(position - glm::vec3(0, 1, 0));
		vertices.push_back(position - glm::vec3(0, -1, 0));

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);
	}

	GLuint get_vertex_buffer() {
		return vertexBuffer;
	}

	glm::vec3& get_position() {
		return position;
	}

	GLuint get_shader_ID() {
		return shaderID;
	}

};