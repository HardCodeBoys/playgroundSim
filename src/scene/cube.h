#pragma once

#include <vector>
#include <iostream>
#include "utils/meth.h"
#include "scene/model.h"

#include "utils/log.h"

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
	Cube(const Cube& other)		
	{ 
		std::cout << "cube copy ctor"; 
		position = other.position;

		vertexBuffer = other.vertexBuffer;
		indexBuffer = other.indexBuffer;

		vertices = other.vertices;
		vertexData = other.vertexData;

		shaderID = other.shaderID;

		CreateVertexData();
		GenerateBuffers();
	}
	Cube(Cube&& other) noexcept 
		: Model(other.position, other.shaderID)
	{ 
		Log::Info("cube move ctor");
		position = other.position;

		vertexBuffer = other.vertexBuffer;
		indexBuffer = other.indexBuffer;

		vertices = other.vertices;
		vertexData = other.vertexData;

		indices = other.indices;

		shaderID = other.shaderID;

		other.Destroy();

		CreateVertexData();
		GenerateBuffers();

	}


	// position is in the middle of the cube
	Cube(const glm::vec3& _position, float size, GLuint _shaderID)
	{
		std::cout << "cube ctor" << std::endl;
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

		CreateVertexData();
		GenerateBuffers();
	}
	~Cube() {}
	
	void CreateVertexData() override {
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
			glm::vec3 normal = meth::CalculateNormal(triangle);
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