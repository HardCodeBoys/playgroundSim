#pragma once

#include "GLHelper.h"

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>

#include "object.h"

class Cube : public Object {
private:
	std::vector<unsigned int> indices = {
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
	std::vector<glm::vec3> normals;
public:
	Cube(const Cube& c)		
	{ 
		std::cout << "cube copy ctor"; 
		vertices = c.vertices;
		shaderID = c.shaderID;
		position = c.position;
		vertexBuffer = c.vertexBuffer;
		generate_buffers();
	}
	Cube(const Cube&& c) { std::cout << "cube move ctor"; }
	// position is in the middle of the cube
	Cube(const glm::vec3& _position, float size, GLuint _shaderID)		
	{
		vertexBuffer = 0;
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

		update_vertex_data();
		generate_buffers();
	}
	glm::vec3 calculate_normal(const std::vector<glm::vec3>& triangle) {
		glm::vec3 normal;
		glm::vec3 u = triangle[1] - triangle[0];
		glm::vec3 v = triangle[2] - triangle[0];

		normal.x = u.y * v.z - u.z * v.y;
		normal.y = u.z * v.x - u.x * v.z;
		normal.z = u.x * v.y - u.y * v.x;

		return normal;
	}
	void update_vertex_data() override {
		int indicesCounter = 0;
		vertexData.clear();
		for (size_t i = 0; i < 12; i++)
		{
			std::vector<glm::vec3> triangle;
			for (size_t j = 0; j < 3; j++)
			{
				triangle.push_back(vertices[indices[indicesCounter]]);
				indicesCounter++;
			}
			glm::vec3 normal = calculate_normal(triangle);
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