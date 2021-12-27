#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

class Model {
public:
	glm::vec3 position;
	std::vector<glm::vec3> vertices;

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertexData;

	GLuint shaderID;

protected:
	GLuint vertexBuffer;
	GLuint indexBuffer;
public:
	Model() { 
		std::cout << "model ctor" << std::endl;
		vertexBuffer = 0;
		indexBuffer = 0;
	}
	Model(const Model& model) { 
		std::cout << "model copy ctor" << std::endl; 
		position = model.position;
		vertices = model.vertices;
		vertexBuffer = model.vertexBuffer;
		indexBuffer = model.indexBuffer;
		indices = model.indices;
		vertexData = model.vertexData;
		shaderID = model.shaderID;
	}
	Model(Model&& model) noexcept { std::cout << "model move ctor" << std::endl; }

	virtual ~Model() {
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
	}
	virtual void UpdateVertexData() = 0;
	void GenerateBuffers();	
	void UpdateBuffers();
	// GETTERS

	GLuint GetVertexBuffer() const { return vertexBuffer; }
	GLuint GetIndexBuffer() const { return indexBuffer; }

	// DEBUG
	void PrintAllVertices() const {
		for (auto&& vertex : vertices) {
			std::cout << glm::to_string(vertex) << std::endl;
		}
	}
};