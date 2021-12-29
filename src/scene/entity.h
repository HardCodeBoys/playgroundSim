#pragma once


#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <memory>
#include <map>

#include "utils/log.h"

#include "scene/component.h"
#include "scene/model.h"
#include "physics/collider.h"
#include "physics/rigid_body.h"


/// <summary>
/// Base class for every entity in the scene, containing information for Rendering, Physics and (AI)
/// </summary>
class Entity {
public:
	glm::vec3 position;

private:
	std::vector<std::shared_ptr<Component>> components;
public:
	Entity(const glm::vec3& _position) : position(_position) { std::cout << "entity ctor" << std::endl; }
	Entity(const Entity& other) { 
		std::cout << "entity copy ctor" << std::endl; 
		position = other.position;
		components = other.components;
	}
	Entity(Entity&& other) noexcept { 
		std::cout << "entity move ctor" << std::endl;
		position = other.position;
		components = other.components;
	}

	~Entity() {
	}

	void PhysicsUpdate(float deltaTime) {
		auto newPos = GetComponent<RigidBody>()->Integrate(deltaTime);
		Move(newPos);
	}

	template <typename T>
	void AddComponent(T&& component) {
		
		static_assert(std::is_base_of<Component, T>::value, "Entity::AddComponent: T must inherit from Component");
		//Log::Info("in add component");
		components.push_back(std::make_shared<T>(std::move(component)));
		//Log::Info("leaving add component, components.size() = \\", components.size());
	}

	template <typename T>
	void RemoveComponent() {
		PL_ERROR("RemoveComponent not implemented");
	}

	template <typename T>
	std::shared_ptr<T> GetComponent() const {
		for (size_t i = 0; i < components.size(); i++)
		{
			static_assert(std::is_base_of<Component, T>::value, "Entity::GetComponent: T must inherit from Component");
			// ?????
			std::string tType = std::string(typeid(T).name()).substr(6);
			//std::cout << "component type = " << components[i]->GetType() << " tType = " << tType << std::endl;
			if (components[i]->GetType() == tType) {
				//Log::Warn("equals");
				return std::dynamic_pointer_cast<T>(components[i]);
			}

		}
		return nullptr;
	}

	void Move(const glm::vec3& direction);

	// -------------------OPERATORS-----------------------
	friend std::ostream& operator<<(std::ostream& os, const Entity& o)
	{
		os  //<< "type: " << typeid(*o.model).name() << std::endl
			<< "vertices.size() " << o.GetComponent<Model>()->vertices.size() << std::endl
			<< "vertexData.size() " << o.GetComponent<Model>()->vertexData.size() << std::endl
			<< "indices.size() " << o.GetComponent<Model>()->indices.size() << std::endl
			<< "position: " << glm::to_string(o.position)
			
			<< std::endl;
		return os;
	}
private:

};