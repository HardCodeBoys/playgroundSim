#pragma once

#include "GLHelper.h"

#include <memory>
#include <map>

#include "renderer.h"

class Scene {
private:
	std::unique_ptr<Renderer> renderer;

	std::map<int, std::unique_ptr<Object>> objects;
	std::map<int, std::unique_ptr<Light>> lights;

	size_t objectCounter;
	size_t lightCounter;
public:
	Scene() :
		renderer(std::make_unique<Renderer>()),
		objectCounter(0), lightCounter(0)
	{

	}
	void create_cube(const glm::vec3& position, float size, const Shader& shader) {
		renderer->add_shader(shader);
		objects.insert(std::make_pair(objectCounter, std::make_unique<Cube>(position, size, shader.get_id())));
		objectCounter++;
	}
	void create_light(const glm::vec3& position, const Shader& shader) {
		renderer->add_shader(shader);
		lights.insert(std::make_pair(lightCounter, std::make_unique<Light>(position, shader.get_id())));
		lightCounter++;
	}
	void create_plane(const glm::vec3& position, float size, const Shader& shader) {
		renderer->add_shader(shader);
		objects.insert(std::make_pair(objectCounter, std::make_unique<Plane>(position, size, shader.get_id())));
		objectCounter++;
	}

	void move_objects(const glm::vec3& direction) {
		for (size_t i = 0; i < objectCounter; ++i) {
			objects[i]->move(direction);
		}
	}


	void render_scene() {
		for (size_t i = 0; i < objectCounter; ++i) {
			renderer->draw_object(objects[i], true);
		}
		for (size_t i = 0; i < lightCounter; ++i) {
			renderer->draw_light(lights[i]);
		}
	}

	// ---------------GETTERS--------------
	std::unique_ptr<Renderer>& get_renderer() {
		return renderer;
	}




	void print_all_objects() {
		for (size_t i = 0; i < objects.size(); i++)
		{
			std::cout << "ID: "<< i << " " << *(objects[i]);
		}
	}
};