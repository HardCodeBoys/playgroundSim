#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "renderer/camera.h"
#include "renderer/shader.h"

#include "scene/light.h"

#include <map>
#include "scene/model.h"
constexpr unsigned int MAX_VERTEX_COUNT = 1000;


class Renderer {
public:
	Camera camera;
private:
	std::map<GLuint, Shader> shaders;

	GLuint VA;
	GLuint VB;
	GLuint IB;


public:
	Renderer() : camera(glm::vec3(4, 0, 7)) {}
	void AddShader(const Shader& shader);
	void Init();
	void foo();

	void DrawModel(const std::shared_ptr<Model>& model, bool withNormals = false);

	void DrawLine(const glm::vec3& a, const glm::vec3& b);
	// should not get a light, just its model
	void DrawLight(const std::unique_ptr<Light>& light);
};