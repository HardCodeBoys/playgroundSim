#pragma once

#include <glm/glm.hpp>

#include "utils/meth.h"
#include "utils/log.h"

#include "physics/box_collider.h"
#include "physics/sphere_collider.h"


class Physics 
{
public:
	// plane
	static bool Raycast(const glm::vec3& originPos, const glm::vec3& direction,
		const glm::vec3& planeNormal, const glm::vec3& planePoint) {

		glm::vec3 points = planePoint - originPos;
		float f = glm::dot(points, planeNormal);
		float denom = glm::dot(direction, planeNormal);
		if (abs(denom) < 0.000001f)
			return false;
		float t = (f / denom);
		if (t > 0)
			return true;
		return false;
	}
	static bool Intersect(const std::shared_ptr<Collider>& a, const std::shared_ptr<Collider>& b) {
		if (a->colliderType == ColliderType::BOX && b->colliderType == ColliderType::BOX) {
			return
				((abs(a->position.x - b->position.x) * 2 <= 2) &&
					(abs(a->position.y - b->position.y) * 2 <= 2) &&
					(abs(a->position.z - b->position.z) * 2 <= 2));
		}
		else if (a->colliderType == ColliderType::BOX && b->colliderType == ColliderType::SPHERE) {
			float dist_squared = b->colliderInfo.size * b->colliderInfo.size;
			glm::vec3 C1 = a->GetBounds()[0];
			glm::vec3 C2 = a->GetBounds()[1];
			glm::vec3 S = b->position;
			if (S.x < C1.x) dist_squared -= meth::Squared(S.x - C1.x);
			else if (S.x > C2.x) dist_squared -= meth::Squared(S.x - C2.x);
			if (S.y < C1.y) dist_squared -= meth::Squared(S.y - C1.y);
			else if (S.y > C2.y) dist_squared -= meth::Squared(S.y - C2.y);
			if (S.z < C1.z) dist_squared -= meth::Squared(S.z - C1.z);
			else if (S.z > C2.z) dist_squared -= meth::Squared(S.z - C2.z);
			return dist_squared > 0;
		}
		else if (a->colliderType == ColliderType::SPHERE && b->colliderType == ColliderType::BOX) {
			Log::Warn("sphere and box");
			float dist_squared = a->colliderInfo.size * a->colliderInfo.size;
			glm::vec3 C1 = b->GetBounds()[0];
			glm::vec3 C2 = b->GetBounds()[1];
			glm::vec3 S = a->position;
			if (S.x < C1.x) dist_squared -= meth::Squared(S.x - C1.x);
			else if (S.x > C2.x) dist_squared -= meth::Squared(S.x - C2.x);
			if (S.y < C1.y) dist_squared -= meth::Squared(S.y - C1.y);
			else if (S.y > C2.y) dist_squared -= meth::Squared(S.y - C2.y);
			if (S.z < C1.z) dist_squared -= meth::Squared(S.z - C1.z);
			else if (S.z > C2.z) dist_squared -= meth::Squared(S.z - C2.z);
			return dist_squared > 0;
		}
		else if (a->colliderType == ColliderType::SPHERE && b->colliderType == ColliderType::SPHERE) {
			float radiusA = a->colliderInfo.size;
			float radiusB = b->colliderInfo.size;

			float dist = abs(meth::Distance(a->position, b->position));
			return dist <= (radiusA + radiusB);
		}
		else
			PL_ERROR("COLLIDERS NOT EXISTING TYPES");
	}
};