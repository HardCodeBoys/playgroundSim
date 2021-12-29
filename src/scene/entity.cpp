#include "playgroundpch.h"

#include "scene/entity.h"

void Entity::Move(const glm::vec3& direction) {
	position += direction;
	for(auto&& component : components) component->Move(direction);
}