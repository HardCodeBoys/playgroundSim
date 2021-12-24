#pragma once

#include "GLHelper.h"

#include <memory>
#include <map>

#include "physics.h"
#include "renderer.h"
#include "gui.h"


class Scene {
public:
	std::unique_ptr<Renderer> renderer;

	GLuint wireframeShader;
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

	void SelectEntity(double xPos, double yPos) {
		// TODO: rework this, written in tutorial style
		float imageAspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		float x = (2.0f * (xPos / SCREEN_WIDTH) - 1.0f);
		float y = 1.0f - (2.0f * (yPos / SCREEN_HEIGHT));
		float z = 1.0f;
		glm::vec3 ray_ndc(x, y, z);
		//std::cout << "Normalised device coordinates = " << glm::to_string(ray_ndc) << std::endl;

		glm::vec4 ray_clip(x, y, -1.0f, 1.0f);
		//std::cout << "4d Homogeneous Clip Coordinates = " << glm::to_string(ray_clip) << std::endl;

		glm::vec4 ray_eye = glm::inverse(renderer->camera.GetProjectionMatrix()) * ray_clip;
		ray_eye.z = -1.0f;
		ray_eye.w =  0.0f;
		//std::cout << "eye space Coordinates = " << glm::to_string(ray_eye) << std::endl;

		glm::vec4 rayWorld = glm::inverse(renderer->camera.GetViewMatrix()) * ray_eye;
		glm::vec3 ray_world(rayWorld.x, rayWorld.y, rayWorld.z);
		ray_world = glm::normalize(ray_world);
		//std::cout << "world coordinates = " << glm::to_string(ray_world) << std::endl;

		glm::vec3 cameraPos = renderer->camera.position;
		glm::vec3 endPos(cameraPos.x + ray_world.x * 50, cameraPos.y + ray_world.y * 50, cameraPos.z + ray_world.z * 50);
		CreateDebugLine(cameraPos, endPos);		

		// for debugging
		/*std::cout << "world coordinates = " << glm::to_string(ray_world) << std::endl;
		std::cout << "hit sphere ? " << Physics::Raycast(renderer->camera.position, ray_world, glm::vec3(0, 1, 1), 0.5f) << std::endl;

		std::cout << "hit plane ? " << Physics::Raycast(renderer->camera.position, ray_world, glm::vec3(0, 1, 0), glm::vec3(0, -1, 0)) << std::endl;
		std::cout << "hit box ? " << Physics::Raycast(renderer->camera.position, ray_world, glm::vec3(-5, -5, -5), 
			glm::vec3(5, 5, 5), true) << std::endl;
		std::cout << "hit box ? " << Physics::Raycast(renderer->camera.position, ray_world, glm::vec3(-0.5f, -0.5f, -0.5f), 
			glm::vec3(0.5f, 0.5f, 0.5f), true) << std::endl;*/

		Ray ray(renderer->camera.position, ray_world);
		float minDistance = 10000.0f;
		int hEntityID = -1;
		bool hit = false;
		for (auto&& entity : entities) {
			if (entity.second->collider != nullptr) {
				hit = entity.second->collider->Raycast(ray);
				if (hit)
				{
					if (ray.distance < minDistance) {
						minDistance = ray.distance;
						hEntityID = entity.first;
					}
				}
			}
		}
		if(hEntityID != -1)
			std::cout << "ID: " << hEntityID << *entities[hEntityID] << std::endl;

	}


	void CreateDebugLine(const glm::vec3& a, const glm::vec3& b) {
		debugLines.push_back(a);
		debugLines.push_back(b);
	}
	void CreateTerrain(const glm::vec3& position, int size, const Shader& shader) {
		auto ptr = std::make_shared<Entity>(
			std::make_shared<Terrain>(position, size, shader.ID));
		entities.insert(std::make_pair(entityCounter, ptr));
		entityCounter++;
	}

	void CreateCube(const glm::vec3& position, float size, const Shader& shader) {
		renderer->AddShader(shader);
		auto ptr = std::make_shared<Entity>(
			std::make_shared<Cube>(position, size, shader.ID),
			std::make_shared<CubeCollider>(position, size,
				std::make_shared<Cube>(position, size*1.2f, wireframeShader)));
		entities.insert(std::make_pair(entityCounter, ptr));
		GUI::AddTable(std::make_shared<EntityTable>(ptr));
		entityCounter++;
	}
	void CreateSphere(const glm::vec3& position, float size, const Shader& shader) {
		renderer->AddShader(shader);
		auto ptr = std::make_shared<Entity>(
				std::make_shared<Sphere>(position, size, shader.ID),
				std::make_shared<SphereCollider>(position, size*2,
					std::make_shared<Cube>(position, size, wireframeShader)));
		entities.insert(std::make_pair(entityCounter, ptr));
		GUI::AddTable(std::make_shared<EntityTable>(ptr));
		entityCounter++;
	}
	void CreatePlane(const glm::vec3& position, float size, const Shader& shader) {
		renderer->AddShader(shader);
		entities.insert(std::make_pair(entityCounter,
					   std::make_shared<Entity>(
				       std::make_shared<Plane>(position, size, shader.ID))));
		entityCounter++;
	}
	void CreateLight(const glm::vec3& position, const Shader& shader) {
		renderer->AddShader(shader);
		lights.insert(std::make_pair(lightCounter, std::make_unique<Light>(position, shader.ID)));
		lightCounter++;
	}
	

	void DeleteEntity(const glm::vec3& position) {
		std::map<int, std::shared_ptr<Entity>>::iterator deletedEntityIt = entities.end();
		for (auto it = entities.begin(); it != entities.end(); ++it) {
			if (it->second->model->position == position)
			{
				deletedEntityIt = it;
			}
		}
		if (deletedEntityIt == entities.end())
			return;
		entities.erase(deletedEntityIt);
	}

	void MoveEntities(const glm::vec3& direction) {
		for (std::map<int, std::shared_ptr<Entity>>::iterator it = entities.begin();
			it != entities.end(); ++it)
		{
			it->second->Move(direction);
		}
	}

	void RenderScene() {
		for (std::map<int, std::shared_ptr<Entity>>::iterator it = entities.begin();
			it != entities.end(); ++it)
		{
			renderer->DrawModel(it->second->model, true);
			if (it->second->collider != nullptr)
				if(it->second->collider->model != nullptr)
					renderer->DrawModel(it->second->collider->model);
		}
		for (std::map<int, std::unique_ptr<Light>>::iterator it = lights.begin();
			it != lights.end(); ++it)
		{
			renderer->DrawLight(it->second);

		}
		for (size_t i = 0; i < debugLines.size(); i += 2)
		{
			renderer->DrawLine(debugLines[i], debugLines[i+1]);
		}
	}

	void PrintAllEntities() {
		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			std::cout << "ID: "<< it->first << " " << *it->second;
		}
	}
};