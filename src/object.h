#pragma once

#include <typeinfo>
#include "GLHelper.h"

#include "camera.h"

class Object {
protected:
	glm::vec3 position;

	std::vector<glm::vec3> vertices;


	GLuint vertexBuffer;

	std::vector<glm::vec3> vertexData;

	GLuint shaderID;
public:
	void move(const glm::vec3& direction) {
		for (size_t i = 0; i < vertices.size(); i++) {
			vertices[i] += direction;
		}
		update_buffers();
	}
	virtual void update_vertex_data() = 0;
	void generate_buffers() {
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW);
	}

	void update_buffers() {
		update_vertex_data();
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertexData.size(), (void*)&vertexData[0]));
	}


	// -------------------GETTERS------------------------

	unsigned int get_num_of_vertices() {
		return vertexData.size() / 2;
	}
	std::vector<glm::vec3> get_vertices() {
		return vertices;
	}
	std::vector<glm::vec3>& get_vertex_data(){
		return vertexData;
	}
	GLuint get_vertex_buffer() const {
		return vertexBuffer;
	}
	GLuint get_shaderID() const {
		return shaderID;
	}
	glm::vec3 get_position() const {
		return position;
	}


	// -------------------OPERATORS-----------------------
	friend std::ostream& operator<<(std::ostream& os, const Object& o)
	{
		os << "Object of type " << typeid(o).name() << " with " << o.vertices.size() << " vertices and "
			<< o.vertexData.size() << " vertexData.size()" << std::endl;
		return os;
	}
};