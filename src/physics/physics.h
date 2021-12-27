#pragma once

#include <glm/glm.hpp>

#include "cube_collider.h"
#include "sphere_collider.h"


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
	// box collider
	static bool Raycast(const glm::vec3& originPos, const glm::vec3& direction,
		const glm::vec3& minBound, const glm::vec3& maxBound, bool box) {

		std::cout << "ray direction" << glm::to_string(direction) << std::endl;
		// the minimum and maximum X values in which the ray could intersect the box /
		float tminx = (minBound.x - originPos.x) / direction.x;
		float tmaxx = (maxBound.x - originPos.x) / direction.x;

		if (tminx > tmaxx) std::swap(tminx, tmaxx);

		float tminy = (minBound.y - originPos.y) / direction.y;
		float tmaxy = (maxBound.y - originPos.y) / direction.y;

		if (tminy > tmaxy) std::swap(tminy, tmaxy);

		if (tminx > tmaxy || tminy > tmaxx)
			return false;
		if (tminy > tminx)
			tminx = tminy;
		if (tmaxy > tmaxx)
			tmaxx = tmaxy;

		float tminz = (minBound.z - originPos.z) / direction.z;
		float tmaxz = (maxBound.z - originPos.z) / direction.z;

		if (tminz > tmaxz) std::swap(tminz, tmaxz);

		if (tminx > tmaxz || tminz > tmaxx)
			return false;

		return true;
	}
	// helper function TODO: move somewhere else
	static bool solveQuadratic(float a, float b, float c, float& x1, float& x2) {
		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0) return false;
		else if (discriminant == 0) x1 = x2 = -0.5f * b / a;
		else {
			float q = (b > 0) ?
				-0.5f * (b + sqrt(discriminant)) :
				-0.5f * (b - sqrt(discriminant));
			x1 = q / a;
			x2 = c / q;
		}
		if (x1 > x2) std::swap(x1, x2);

		return true;
	}

	// sphere collider
	static bool Raycast(const glm::vec3& rayOrigin, const glm::vec3 rayDirection, const glm::vec3& spherePosition, float radius) {
		glm::vec3 L = spherePosition - rayOrigin;
		float a = glm::dot(rayDirection, rayDirection);
		float b = 2 * glm::dot(rayDirection, rayOrigin - spherePosition);
		float c = glm::dot(rayOrigin - spherePosition, rayOrigin - spherePosition) - (radius * radius);
		float x1 = 0, x2 = 0;
		if (solveQuadratic(a, b, c, x1, x2)) return true;
		// stupid, to indicate that the ray can return which of the points is closer to the origin
		return false;
	}
};