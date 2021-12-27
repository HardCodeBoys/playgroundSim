#include "playgroundpch.h"
#include "camera.h"

glm::mat4 Camera::GetProjectionMatrix() const {
	return projectionMatrix;
}
glm::mat4 Camera::GetViewMatrix() const {
	glm::mat4 viewMatrix = glm::lookAt(
		position,
		position + direction,
		glm::vec3(0, 1, 0)
	);
	return viewMatrix;
}


void Camera::RotateCamera(float deltaTime, float x, float y) {
	horizontalAngle += MOUSE_SPEED * deltaTime * float(SCREEN_WIDTH / 2 - x);
	verticalAngle += MOUSE_SPEED * deltaTime * float(SCREEN_HEIGHT / 2 - y);
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
}

glm::mat4 Camera::CalculateMVP(const glm::vec3& modelPosition) const {

	glm::mat4 viewMatrix = glm::lookAt(
		position,
		position + direction,
		glm::vec3(0, 1, 0)
	);
	glm::mat4 model = glm::translate(glm::mat4(1.f), modelPosition);


	return projectionMatrix * viewMatrix * model;
}

void Camera::MoveForward(float deltaTime) {
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	position += direction * deltaTime * CAMERA_SPEED;
}
void Camera::MoveBackwards(float deltaTime) {
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	position -= direction * deltaTime * CAMERA_SPEED;
}
void Camera::StrafeLeft(float deltaTime) {
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	position -= right * deltaTime * CAMERA_SPEED;
}
void Camera::StrafeRight(float deltaTime) {
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	position += right * deltaTime * CAMERA_SPEED;
}