#pragma once

#include "GLHelper.h"
#include "model.h"

struct Ray
{
	Ray(const glm::vec3& o, const glm::vec3& d) : origin(o), direction(d), distance(0) {}
	glm::vec3 origin;
	glm::vec3 direction;
	float distance;
};


class Collider
{
public:
	glm::vec3 position;
	// model for debugging purposes, will probably delete later
	std::shared_ptr<Model> model;
private:
public:
	virtual bool Raycast(Ray& ray) = 0;
	virtual bool Intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) = 0;
	virtual bool Intersect(const std::shared_ptr<Collider>& other) = 0;


};