#pragma once

#include <glm/glm.hpp>
#include "scene/component.h"

class RigidBody : public Component
{
private:
	glm::vec3 position;
	glm::vec3 velocity;

	// 0 if static, 1 if not
	bool isStatic;
public:
	RigidBody()
		: Component("RigidBody")
	{}
	RigidBody(const glm::vec3& _position, bool _isStatic)
		: Component("RigidBody"), position(_position), velocity({ 0, -1.81f, 0 }), isStatic(_isStatic)
	{
		
	}

	glm::vec3 Integrate(float deltaTime) {
		// returns the direction in which the rigidbody moves
		return velocity * deltaTime;
	}
	glm::vec3 GetVelocity() const { return velocity; }

	void SetVelocity(const glm::vec3& _velocity) {
		velocity = _velocity;
	}

	bool IsStatic() const { return isStatic; }
	void Move(const glm::vec3& direction) override {
		position += direction;
	}
};