#include "playgroundpch.h"

#include "scene/entity.h"

void Entity::Move(const glm::vec3& direction) {
	position += direction;
	for (size_t i = 0; i < model->vertices.size(); i++) {
		model->vertices[i] += direction;
	}
	model->position += direction;

	if (collider != nullptr) {
		if (collider->model != nullptr)
		{
			for (size_t i = 0; i < collider->model->vertices.size(); i++)
			{
				collider->model->vertices[i] += direction;
			}
			collider->position += direction;
			collider->model->position += direction;
			collider->model->UpdateBuffers();
		}
	}
	model->UpdateBuffers();
}