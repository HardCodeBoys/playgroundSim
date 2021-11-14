#pragma once
#include "GLHelper.h"



#define CAMERA_SPEED 5.f
#define MOUSE_SPEED 0.001f


class Camera {
private:
	glm::vec3 position;
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
		verticalAngle(0.f)
	{
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	}

	
	void set_position(const glm::vec3& _position) {
		position = _position;
	}
	glm::vec3 get_position() const {
		return position;
	}

	glm::mat4 get_projection_matrix() const {
		return projectionMatrix;
	}
	glm::mat4 get_view_matrix() const {
		glm::mat4 viewMatrix = glm::lookAt(
			position,
			position + direction,
			glm::vec3(0, 1, 0)
		);
		return viewMatrix;
	}


	void rotate_camera(float deltaTime, float x, float y) {
		horizontalAngle += MOUSE_SPEED * deltaTime * float(SCREEN_WIDTH / 2 - x);
		verticalAngle += MOUSE_SPEED * deltaTime * float(SCREEN_HEIGHT / 2 - y);
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
	}

	glm::mat4 calculate_mvp(const glm::vec3& modelPosition) {

		glm::mat4 viewMatrix = glm::lookAt(
			position,
			position + direction,
			glm::vec3(0, 1, 0)
		);
		glm::mat4 model = glm::translate(glm::mat4(1.f), modelPosition);


		return projectionMatrix * viewMatrix * model;
	}

	void move_forward(float deltaTime) {
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		position += direction * deltaTime * CAMERA_SPEED;
	}
	void move_backwards(float deltaTime) {
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		position -= direction * deltaTime * CAMERA_SPEED;
	}
	void strafe_left(float deltaTime) {
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		position -= right * deltaTime * CAMERA_SPEED;
	}
	void strafe_right(float deltaTime) {
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		position += right * deltaTime * CAMERA_SPEED;
	}

	void print() {
		std::cout << "camera at position " << glm::to_string(position) << std::endl;
	}
};