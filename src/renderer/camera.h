#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "core/base.h"

constexpr float CAMERA_SPEED = 5.f;
constexpr float MOUSE_SPEED = 0.001f;

class Camera {
public:
	glm::vec3 position;
private:
	glm::mat4 projectionMatrix;
	float mouseX;
	float mouseY;

	float horizontalAngle;
	float verticalAngle;

	glm::vec3 direction;
	glm::vec3 right;
public:
	Camera(const glm::vec3& _position) :
		position(_position),
		horizontalAngle(3.14f),
		verticalAngle(0.0f)
	{
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	}
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;


	void RotateCamera(float deltaTime, float x, float y);

	glm::mat4 CalculateMVP(const glm::vec3& modelPosition) const;

	void MoveForward(float deltaTime);
	void MoveBackwards(float deltaTime);
	void StrafeLeft(float deltaTime);
	void StrafeRight(float deltaTime);

	const void Print() const;
};