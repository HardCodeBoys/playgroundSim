#pragma once

#include "collider.h"

class CubeCollider : public Collider
{
	
public:
	float size;

	CubeCollider(const glm::vec3& _position, float _size, const std::shared_ptr<Model>& _model) {
		position = _position;
		size = _size;
		model = _model;
	}
	bool Raycast(Ray& ray) override {
		glm::vec3 rayOrigin = ray.origin;
		glm::vec3 rayDirection = ray.direction;

		glm::vec3 minBound((*(model->vertices.begin())));
		glm::vec3 maxBound(*(model->vertices.end() - 1));

		glm::vec3 bounds[2] = { minBound, maxBound };

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
	bool Intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) override {
		std::cout << "ERROR: not implemented" << std::endl;
		return false;
	}
	bool Intersect(const std::shared_ptr<Collider>& other) override {
		std::cout << "ERROR: not implemented" << std::endl;
		return false;
	}
};