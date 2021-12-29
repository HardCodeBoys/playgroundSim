#include "playgroundpch.h"

#include "scene/model.h"

void Model::GenerateBuffers() {

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
void Model::UpdateBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertexData.size(), (void*)vertexData.data());
	// indices might stay the same most of the time, just the vertices are changing
	if (indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * indices.size(), (void*)indices.data());
	}

}