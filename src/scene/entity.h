#pragma once


#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <memory>

#include "scene/model.h"
#include "physics/collider.h"

/// <summary>
/// Base class for every entity in the scene, containing information for Rendering, Physics and (AI)
/// </summary>
class Entity {
public:
	glm::vec3 position;

	// for rendering
	std::shared_ptr<Model> model;

	std::shared_ptr<Collider> collider;
protected:
	/*model
	collider (rigidBody)
	AI*/	
public:
	Entity() { std::cout << "entity ctor" << std::endl; }
	Entity(const Entity& entity) { 
		std::cout << "entity copy ctor" << std::endl; 
		model = entity.model;
		position = entity.position;
	}
	Entity(Entity&& entity) noexcept { std::cout << "entity move ctor" << std::endl; }
	Entity(const std::shared_ptr<Model>&& _model) {
		std::cout << "entity model ctor" << std::endl;
		position = _model->position;
		model = _model;
	}
	// move ??? or not
	Entity(std::shared_ptr<Model>&& _model, std::shared_ptr<Collider>&& _collider) {
		std::cout << "entity collider, model ctor" << std::endl;
		position = _model->position;
		model = _model;
		collider = _collider;
	}
	void Move(const glm::vec3& direction);

	// -------------------OPERATORS-----------------------
	friend std::ostream& operator<<(std::ostream& os, const Entity& o)
	{
		os  << "type: " << typeid(*o.model).name() << std::endl
			<< "vertices.size() " << o.model->vertices.size() << std::endl
			<< "vertexData.size() " << o.model->vertexData.size() << std::endl
			<< "indices.size() " << o.model->indices.size() << std::endl
			<< "position: " << glm::to_string(o.position)
			
			<< std::endl;
		return os;
	}
};