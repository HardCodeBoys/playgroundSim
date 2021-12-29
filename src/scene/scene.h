#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <map>
#include <memory>

#include "core/base.h"
#include "renderer/renderer.h"
#include "gui/gui.h"
#include "physics/physics.h"

#include "physics/rigid_body.h"

#include "scene/model.h"
#include "scene/entity.h"
#include "scene/cube.h"
#include "scene/sphere.h"
#include "scene/terrain.h"
#include "scene/plane.h"



class Scene {
public:
	std::unique_ptr<Renderer> renderer;

	GLuint WIREFRAME_SHADER;
	GLuint BASIC_SHADER;
private:
	std::map<int, std::shared_ptr<Entity>> entities;
	std::map<int, std::unique_ptr<Light>> lights;

	// stride = 2
	std::vector<glm::vec3> debugLines;

	size_t entityCounter;
	size_t lightCounter;

	int selectedEntity;
public:
	Scene() :
		renderer(std::make_unique<Renderer>()),
		entityCounter(0), lightCounter(0)
	{
	}

	void SelectEntity(double xPos, double yPos);

	void CreateDebugLine(const glm::vec3& a, const glm::vec3& b);

	void CreateTerrain(const glm::vec3& position, int size, const Shader& shader);
	void CreateCube(const glm::vec3& position, float size, const Shader& shader);
	void CreateSphere(const glm::vec3& position, float size, const Shader& shader);
	void CreatePlane(const glm::vec3& position, float size, const Shader& shader);
	void CreateLight(const glm::vec3& position, const Shader& shader);

	void DeleteEntity(const glm::vec3& position);

	void MoveEntities(const glm::vec3& direction);

	void UpdateScene(float deltaTime);

	void RenderScene();

	void DrawGizmos();

	void PrintAllEntities();
};