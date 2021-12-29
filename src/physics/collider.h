#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "scene/model.h"
#include "scene/component.h"
struct Ray
{
	Ray(const glm::vec3& o, const glm::vec3& d) : origin(o), direction(d), distance(0) {}
	glm::vec3 origin;
	glm::vec3 direction;
	float distance;
};

enum class ColliderType {
	BOX,
	SPHERE
};

struct ColliderInfo {
	float size;
	std::vector<glm::vec3> bounds;
};

class Collider : public Component
{
public:
	glm::vec3 position;
	std::shared_ptr<Model> gizmo;
	ColliderType colliderType;
	ColliderInfo colliderInfo;
protected:
public:
	Collider()
		: Component("Collider")
	{
		std::cout << "collider component type ctor" << std::endl;
	}
	Collider(const glm::vec3& _position) 
		: Component("Collider"), position(_position)
	{
		std::cout << "collider component type and position ctor" << std::endl;
	}
	Collider(const Collider& other) 
		: Component("Collider")
	{
		std::cout << "collider copy ctor" << std::endl;
	}
	Collider(Collider&& other) 
		: Component("Collider")
	{
		std::cout << "collider move ctor" << std::endl;

	}
	virtual ~Collider() {
		std::cout << "collider dtor" << std::endl;
	}

	std::shared_ptr<Model> GetGizmo() const {
		return gizmo;
	}
	virtual void Move(const glm::vec3& direction) override = 0;

	virtual std::vector<glm::vec3>& GetBounds() = 0;

	virtual bool Raycast(Ray& ray) = 0;


};