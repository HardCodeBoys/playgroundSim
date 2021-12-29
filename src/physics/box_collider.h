#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "physics/collider.h"
#include "scene/cube.h"


class BoxCollider : public Collider
{
private:
	std::vector<glm::vec3> bounds;
	float size;
public:

	BoxCollider(const glm::vec3& _position, float _size, GLuint shaderID)
		: Collider(_position), size(_size)
	{
		std::cout << "BoxCollider same size ctor" << std::endl;
		colliderType = ColliderType::BOX;
		float d = size * 0.5f;
		bounds.push_back( { position.x - d, position.y - d, position.z - d } );
		bounds.push_back( { position.x + d, position.y + d, position.z + d } );

		colliderInfo = { _size, {bounds[0], bounds[1]} };

		gizmo = std::make_unique<Cube>(position, size, shaderID);

	}
	BoxCollider(const glm::vec3& _position, float sizeX, float sizeY, float sizeZ) {
		std::cout << "BoxCollider different size ctor" << std::endl;
		position = _position;
	}
	BoxCollider(const BoxCollider& other) {
		std::cout << "BoxCollider copy ctor" << std::endl;
		position = other.position;
	}
	// currently used
	BoxCollider(BoxCollider&& other) noexcept 
		: Collider(other.position), size(other.size)
	{
		std::cout << "BoxCollider move ctor" << std::endl;
		colliderType = ColliderType::BOX;
		colliderInfo = other.colliderInfo;
		bounds.push_back(other.bounds[0]);
		bounds.push_back(other.bounds[1]);
		gizmo = other.gizmo;
	}
	~BoxCollider() {
		std::cout << "BoxCollider dtor" << std::endl;
	}

	bool Raycast(Ray& ray) override {
		glm::vec3 rayOrigin = ray.origin;
		glm::vec3 rayDirection = ray.direction;
		
		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		glm::vec3 invDir(1 / rayDirection.x, 1 / rayDirection.y, 1 / rayDirection.z);
		int signX = (invDir.x < 0);
		int signY = (invDir.y < 0);
		int signZ = (invDir.z < 0);

		tmin = (bounds[signX].x - rayOrigin.x) * invDir.x;
		tmax = (bounds[1 - signX].x - rayOrigin.x) * invDir.x;
		tymin = (bounds[signY].y - rayOrigin.y) * invDir.y;
		tymax = (bounds[1 - signY].y - rayOrigin.y) * invDir.y;

		if ((tmin > tymax) || (tymin > tmax)) return false;
		if (tymin > tmin) tmin = tymin;
		if (tymax < tmax) tmax = tymax;

		tzmin = (bounds[signZ].z - rayOrigin.z) * invDir.z;
		tzmax = (bounds[1 - signZ].z - rayOrigin.z) * invDir.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		glm::vec3 pointHit = rayOrigin + rayDirection * tmin;
		
		float distance = glm::distance(pointHit, rayOrigin);

		ray.distance = distance;

		return true;
	}
	void Move(const glm::vec3& direction) override {
		position += direction;
		bounds[0] += direction;
		bounds[1] += direction;
		gizmo->Move(direction);
	}
	std::vector<glm::vec3>& GetBounds() override {
		return bounds;
	}
};