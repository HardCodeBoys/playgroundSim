#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

#include "scene/component.h"

class Model : public Component {
public:
	glm::vec3 position;
	std::vector<glm::vec3> vertices;

	std::vector<unsigned int> indices;
	// [vertex, normal]
	std::vector<glm::vec3> vertexData;

	GLuint shaderID;

protected:
	GLuint vertexBuffer;
	GLuint indexBuffer;
public:
	Model() 
		: Component("Model")
	{ 
		std::cout << "model ctor" << std::endl;		
	}
	Model(const glm::vec3& _position, GLuint _shaderID)
		: Component("Model"), position(_position), vertexBuffer(0), indexBuffer(0), shaderID(_shaderID)
	{
		std::cout << "model position and shaderID ctor" << std::endl;

	}
	Model(const Model& other) 
		: Component("Model")
	{ 
		std::cout << "model copy ctor" << std::endl; 
		position = other.position;
		vertices = other.vertices;
		vertexBuffer = other.vertexBuffer;
		indexBuffer = other.indexBuffer;
		indices = other.indices;
		vertexData = other.vertexData;
		shaderID = other.shaderID;
	}
	Model(Model&& model) noexcept 
		: Component("Model")
	{
		std::cout << "model move ctor" << std::endl; 
	}

	virtual void Destroy() {
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
		// ???
		vertexBuffer = -1;
		indexBuffer = -1;
		shaderID = -1;

		position = { 0, 0, 0 };

		vertices.clear();
		indices.clear();

		vertexData.clear();
	}

	virtual ~Model() {
		std::cout << "model dtor" << std::endl;
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
	}

	void Move(const glm::vec3& direction) override {
		for (size_t i = 0; i < vertices.size(); i++) {
			vertices[i] += direction;
		}
		position += direction;
		MoveVertexData(direction);
		UpdateBuffers();
	}
	void MoveVertexData(const glm::vec3& direction) {
		// move every second vertex because of stride [vertex, normal]
		for (size_t i = 0; i < vertexData.size(); i += 2)
		{
			vertexData[i] += direction;
		}
	}
	virtual void CreateVertexData() = 0;
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