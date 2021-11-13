#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>

#include "mesh.h"

class Cube : public Mesh {
private:
	glm::vec3 position;

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;

	std::vector<glm::vec3> vertexData;

	GLuint vertexBuffer;
	GLuint normalsBuffer;
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
		generate_buffers();
	}
	std::vector<glm::vec3> get_normals() {
		return normals;
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
	void update_vertex_data() {
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

	void generate_buffers() {
		update_vertex_data();

		GLCall(glGenBuffers(1, &vertexBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW));
	}


	void update_buffers() {
		update_vertex_data();
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertexData.size(), (void*) &vertexData[0]));
	}

	glm::vec3 get_position() const {
		return position;
	}
	std::vector<glm::vec3>& get_vertex_data() {
		return vertexData;
	}
	std::vector<glm::vec3>& get_vertices() {
		return vertices;
	}
	void set_indices() {
		indices = std::vector<unsigned int>{
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
	}
	void set_index_buffer(GLuint _indexBuffer) {
		indexBuffer = _indexBuffer;
	}
	GLuint get_index_buffer() {
		return indexBuffer;
	}
	GLuint get_normals_buffer() {
		return normalsBuffer;
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
	GLuint get_shader_ID() {
		return shaderID;
	}
	void move(const glm::vec3& t) {
		for (auto&& v : vertices) {
			v += t;
		}
		update_buffers();
	}

	void print_vertex_data() {
		std::cout << "size of vertexData = " << vertexData.size() << std::endl;
		for (size_t i = 0; i < vertexData.size(); i += 2)
		{
			std::cout << "vertex = " << glm::to_string(vertexData[i]) << "with normal: " << glm::to_string(vertexData[i + 1]) << std::endl;
		}
	}
	void print_vertices() {
		for (auto&& v : vertices) {
			std::cout << glm::to_string(v)  << std::endl;
		}
		std::cout << std::endl;
	}

};