#pragma once
#include "GLHelper.h"

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
	void GenerateBuffers() {

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		unsigned int stride = sizeof(glm::vec3) * 2;
		// vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

		// normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, stride, (void*)(sizeof(glm::vec3)));

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexData.size(), vertexData.data(), GL_DYNAMIC_DRAW);
		if (indices.size() > 0) {
			glGenBuffers(1, &indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
		}

	}
	void UpdateBuffers() {
		UpdateVertexData();
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertexData.size(), (void*)vertexData.data());
		// indices might stay the same most of the time, just the vertices are changing
		if (indices.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * indices.size(), (void*)indices.data());
		}

	}
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