#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
class Plane {
private:
	glm::vec3 position;
	std::vector<glm::vec3> vertices;

	std::vector<glm::vec3> vertexData;

	GLuint vertexBuffer;
	GLuint shaderID;
public:
	Plane(const glm::vec3& _position, float size, GLuint _shaderID) : position(_position), shaderID(_shaderID), vertexBuffer(0) {
		float d = size / 2;
		vertices.push_back(glm::vec3(-d, 0, -d));
		vertices.push_back(glm::vec3(d, 0, -d));
		vertices.push_back(glm::vec3(d, 0, d));

		vertices.push_back(glm::vec3(d, 0, d));
		vertices.push_back(glm::vec3(-d, 0, d));
		vertices.push_back(glm::vec3(-d, 0, -d));

		generate_buffers();
	}

	GLuint get_vertex_buffer() {
		return vertexBuffer;
	}

	glm::vec3 get_position() {
		return position;
	}
	GLuint get_shader_id() {
		return shaderID;
	}

	void generate_buffers() {
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertexData.push_back(vertices[i]);
			vertexData.push_back(glm::vec3(0, 1, 0));
		}

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW);
	}
};