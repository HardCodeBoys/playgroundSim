#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
class Light {
public:
	glm::vec3 position;

	std::vector<glm::vec3> vertices;

	GLuint shaderID;
private:
	GLuint vertexBuffer;
public:
	Light(const glm::vec3& _position, GLuint shader) :
		position(_position), shaderID(shader)
	{
		vertices.push_back(position - glm::vec3(-1, 0, 0));
		vertices.push_back(position - glm::vec3(1, 0, 0));
		vertices.push_back(position - glm::vec3(0, 1, 0));

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);
	}

	GLuint GetVertexBuffer() const {
		return vertexBuffer;
	}
};