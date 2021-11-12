#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "mesh.h"

class Cube : public Mesh {
private:
	glm::vec3 position;

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;

	GLuint vertexBuffer;
	GLuint indexBuffer;

	GLuint shaderID;
public:
	// position is in the middle of the cube
	Cube(const glm::vec3& _position, float size, GLuint shader) :
		position(_position), vertexBuffer(0), indexBuffer(0)
	{
		float d = size / 2;
		vertices.push_back(position + glm::vec3(-d, -d, -d));
		vertices.push_back(position + glm::vec3(d, -d, -d));
		vertices.push_back(position + glm::vec3(-d, d, -d));
		vertices.push_back(position + glm::vec3(d, d, -d));

		vertices.push_back(position + glm::vec3(-d, -d, d));
		vertices.push_back(position + glm::vec3(d, -d, d));
		vertices.push_back(position + glm::vec3(-d, d, d));
		vertices.push_back(position + glm::vec3(d, d, d));

		shaderID = shader;
		
		set_indices();
		calculate_normals();
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
	void calculate_normals() {
		int counter = 0;
		for (size_t i =	0; i < 12; i++)
		{
			std::vector<glm::vec3> triangle;
			for (size_t i = 0; i < 3; i++)
			{
				triangle.push_back(vertices[indices[i + counter]]);
			}
			counter +=3;
			normals.push_back(calculate_normal(triangle));
		}
	}
	void generate_buffers() {
		std::vector<glm::vec3> vertexData;
		for (size_t i = 0; i < 8; i++)
		{
			vertexData.push_back(vertices[i]);
			vertexData.push_back(normals[i]);
		}

		GLCall(glGenBuffers(1, &vertexBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW));

		GLCall(glGenBuffers(1, &indexBuffer));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_DYNAMIC_DRAW));
	}


	void update_buffers() {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertices.size(), (void*) &vertices[0]));
	}

	glm::vec3 get_position() const {
		return position;
	}
	std::vector<glm::vec3>& get_vertices() {
		return vertices;
	}
	void set_indices() {
		indices = std::vector<unsigned int>{
			0, 1, 2,
			2, 1, 3,

			1, 3, 5,
			3, 5, 7,

			0, 4, 2,
			4, 2, 6,

			0, 1, 4,
			1, 4, 5,

			2, 3, 6,
			3, 6, 7,

			5, 4, 6,
			6, 5, 7
		};
	}
	void set_index_buffer(GLuint _indexBuffer) {
		indexBuffer = _indexBuffer;
	}
	GLuint get_index_buffer() {
		return indexBuffer;
	}
	GLuint get_vertex_buffer() {
		return vertexBuffer;
	}
	std::vector<unsigned int>& get_indices() {
		return indices;
	}

	void set_shader(GLuint _shaderID) {
		shaderID = _shaderID;
	}
	GLuint get_shader() {
		return shaderID;
	}
	void move(const glm::vec3& t) {
		for (auto&& v : vertices) {
			v += t;
			std::cout << "moving up to " << glm::to_string(v) << std::endl;
		}
		update_buffers();
	}

	void print() {
		for (auto&& v : vertices) {
			std::cout << glm::to_string(v)  << std::endl;
		}
		std::cout << std::endl;
	}

};