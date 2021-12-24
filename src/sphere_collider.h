#pragma once

#include "GLHelper.h"
#include "collider.h"

class SphereCollider : public Collider
{
public:
	float radius;
	SphereCollider(const glm::vec3& _position, float _size, const std::shared_ptr<Model>& _model) {
		position = _position;
		radius = _size / 4;
		model = _model;
	}
	bool Raycast(Ray& ray) override {
		glm::vec3 rayOrigin = ray.origin;
		glm::vec3 rayDirection = ray.direction;

		glm::vec3 L = position - rayOrigin;
		float a = glm::dot(rayDirection, rayDirection);
		float b = 2.0f * glm::dot(L, rayDirection);
		float c = glm::dot(L, L) - (radius * radius);
		float x1 = 0, x2 = 0;
		if (meth::SolveQuadratic(a, b, c, x1, x2)) return true;
		// stupid, to indicate that the ray can return which of the points is closer to the origin
		return false;
	}
	bool Intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) override {
		std::cout << "ERROR: not implemented" << std::endl;
		return false;
	}
	bool Intersect(const std::shared_ptr<Collider>& other) override {
		std::cout << "ERROR: not implemented" << std::endl;
		return false;
	}
	 
};