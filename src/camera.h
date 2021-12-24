#pragma once
#include "GLHelper.h"

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
	glm::mat4 GetProjectionMatrix() const {
		return projectionMatrix;
	}
	glm::mat4 GetViewMatrix() const {
		glm::mat4 viewMatrix = glm::lookAt(
			position,
			position + direction,
			glm::vec3(0, 1, 0)
		);
		return viewMatrix;
	}


	void RotateCamera(float deltaTime, float x, float y) {
		horizontalAngle += MOUSE_SPEED * deltaTime * float(SCREEN_WIDTH / 2 - x);
		verticalAngle += MOUSE_SPEED * deltaTime * float(SCREEN_HEIGHT / 2 - y);
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
	}

	glm::mat4 CalculateMVP(const glm::vec3& modelPosition) const {

		glm::mat4 viewMatrix = glm::lookAt(
			position,
			position + direction,
			glm::vec3(0, 1, 0)
		);
		glm::mat4 model = glm::translate(glm::mat4(1.f), modelPosition);


		return projectionMatrix * viewMatrix * model;
	}

	void MoveForward(float deltaTime) {
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		position += direction * deltaTime * CAMERA_SPEED;
	}
	void MoveBackwards(float deltaTime) {
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		position -= direction * deltaTime * CAMERA_SPEED;
	}
	void StrafeLeft(float deltaTime) {
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		position -= right * deltaTime * CAMERA_SPEED;
	}
	void StrafeRight(float deltaTime) {
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		position += right * deltaTime * CAMERA_SPEED;
	}

	const void Print() const {
		std::cout << "camera at position " << glm::to_string(position) << std::endl;
	}
};