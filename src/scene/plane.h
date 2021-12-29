#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene/model.h"

class Plane : public Model {
private:
public:
	Plane(const glm::vec3& _position, float size, GLuint _shaderID) {
		vertexBuffer = 0;
		position = _position;
		shaderID = _shaderID;
		float d = size * 0.5f;
		vertices.push_back(position + glm::vec3(-d, 0, -d));
		vertices.push_back(position + glm::vec3(d, 0, -d));
		vertices.push_back(position + glm::vec3(d, 0, d));
						   
		vertices.push_back(position + glm::vec3(d, 0, d));
		vertices.push_back(position + glm::vec3(-d, 0, d));
		vertices.push_back(position + glm::vec3(-d, 0, -d));

		CreateVertexData();
		GenerateBuffers();
	}
	void CreateVertexData() override {
		vertexData.clear();
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertexData.push_back(vertices[i]);
			// hardcoded normals
			vertexData.push_back(glm::vec3(0, 1, 0));
		}
	}
};