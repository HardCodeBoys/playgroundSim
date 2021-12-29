#pragma once

#include <glm/glm.hpp>
#include "utils/meth.h"
#include "collider.h"
#include "scene/sphere.h"
class SphereCollider : public Collider
{
public:
	float radius;
	SphereCollider(const glm::vec3& _position, float _size, GLuint shaderID) {
		colliderType = ColliderType::SPHERE;
		position = _position;
		radius = _size / 2;
		colliderInfo = { _size / 2, { {0, 0, 0}, {0, 0, 0} } };

		gizmo = std::make_unique<Sphere>(position, _size, shaderID);
	}
	SphereCollider(SphereCollider&& other) noexcept {
		std::cout << "SphereCollider move ctor" << std::endl;
		position = other.position;
		colliderType = ColliderType::SPHERE;
		colliderInfo = other.colliderInfo;
		gizmo = other.gizmo;
	}
	bool Raycast(Ray& ray) override {
		glm::vec3 rayOrigin = ray.origin;
		glm::vec3 rayDirection = ray.direction;

		glm::vec3 L = position - rayOrigin;
		float a = glm::dot(rayDirection, rayDirection);
		float b = 2.0f * glm::dot(L, rayDirection);
		float c = glm::dot(L, L) - (colliderInfo.size * colliderInfo.size);
		float x1 = 0, x2 = 0;
		Log::Warn("a = \\, b = \\, c = \\, x1 = \\, x2 = \\", a, b, c, x1, x2);
		if (meth::SolveQuadratic(a, b, c, x1, x2)) return true;
		// stupid, to indicate that the ray can return which of the points is closer to the origin
		return false;
	}
	std::vector<glm::vec3>& GetBounds()override {
		PL_NOT_IMPLEMENTED();
		std::vector<glm::vec3> temp;
		return temp;
	}
	void Move(const glm::vec3& direction) override {
		position += direction;
		gizmo->Move(direction);
	}
};