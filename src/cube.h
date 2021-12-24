#pragma once

#include "GLHelper.h"

#include <iostream>
#include <vector>
#include "playground.h"
#include "meth.h"

#include "model.h"

class Cube : public Model {
private:
	std::vector<unsigned int> index = {
		//front
		0, 2, 3,
		3, 1, 0,
		//right
		7, 5, 1,
		1, 3, 7,
		//left
		6, 2, 0,
		0, 4, 6,
		//bottom
		0, 1, 5,
		5, 4, 0,
		//top
		2, 6, 7,
		7, 3, 2,
		//back
		4, 5, 7,
		7, 6, 4 
	};
public:
	Cube(const Cube& c)		
	{ 
		std::cout << "cube copy ctor"; 
		vertices = c.vertices;
		shaderID = c.shaderID;
		position = c.position;
		vertexBuffer = c.vertexBuffer;

		

		GenerateBuffers();
	}
	Cube(const Cube&& c) noexcept { std::cout << "cube move ctor"; }
	// position is in the middle of the cube
	Cube(const glm::vec3& _position, float size, GLuint _shaderID)		
	{
		position = _position;
		shaderID = _shaderID;

		float d = size / 2;
		vertices.push_back(position + glm::vec3(-d, -d, -d));
		vertices.push_back(position + glm::vec3(d, -d, -d));
		vertices.push_back(position + glm::vec3(-d, d, -d));
		vertices.push_back(position + glm::vec3(d, d, -d));

		vertices.push_back(position + glm::vec3(-d, -d, d));
		vertices.push_back(position + glm::vec3(d, -d, d));
		vertices.push_back(position + glm::vec3(-d, d, d));
		vertices.push_back(position + glm::vec3(d, d, d));

		UpdateVertexData();
		GenerateBuffers();
	}
	static glm::vec3 CalculateNormal(const std::vector<glm::vec3>& triangle) {
		glm::vec3 normal(0, 0, 0);
		glm::vec3 u = triangle[1] - triangle[0];
		glm::vec3 v = triangle[2] - triangle[0];

		normal.x = u.y * v.z - u.z * v.y;
		normal.y = u.z * v.x - u.x * v.z;
		normal.z = u.x * v.y - u.y * v.x;

		return normal;
	}
	void UpdateVertexData() override {
		vertexData.clear();

		int indicesCounter = 0;
		std::vector<glm::vec3> normals;
		for (size_t i = 0; i < 12; i++)
		{
			std::vector<glm::vec3> triangle;
			for (size_t j = 0; j < 3; j++)
			{
				triangle.push_back(vertices[index[indicesCounter]]);
				indicesCounter++;
			}
			glm::vec3 normal = CalculateNormal(triangle);
			//glm::vec3 normal = glm::triangleNormal(triangle[0], triangle[1], triangle[2]);
			
			for (size_t t = 0; t < 3; t++)
			{
				vertexData.push_back(triangle[t]);
				vertexData.push_back(normal);
				normals.push_back(normal);
			}
		}
	}
};