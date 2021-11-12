#pragma once
#include <glm/glm.hpp>

#include <map>
#include <set>
#include <memory>

#include "GLHelper.h"
#include "mesh/mesh.h"
#include "mesh/cube.h"
#include "camera.h"
#include "light.h"

class Renderer {
private:
	std::map<int, std::unique_ptr<Cube>> cubes;
	std::map<int, std::unique_ptr<Light>> lights;

	std::set<GLuint> shaders;

	int cubeCounter;
	int lightCounter;

	GLuint colorBuffer;
	Camera camera;

public:
	Renderer() : camera(glm::vec3(4, 0, 7)), cubeCounter(0), lightCounter(0) {}
	void set_camera_position(const glm::vec3& _cameraPosition) {
		camera.set_position(_cameraPosition);
	}
	void set_color_buffer(GLuint _colorBuffer) {
		colorBuffer = _colorBuffer;
	}
	Camera& get_camera() {
		return camera;
	}
	glm::vec3& get_camera_position() {
		return camera.get_position();
	}

	void draw_scene() {

	}
	void move_cubes(const glm::vec3& vec) {
		for (auto&& cube : cubes)
		{
			cube.second->move(vec);
		}
	}
	void add_shader(GLuint id) {
		shaders.insert(id);
	}

	void create_cube(const glm::vec3& position, float size, GLuint shader) {
		Cube cube(position, size, shader);
		cubes.insert(std::make_pair(cubeCounter, std::make_unique<Cube>(cube)));
		cubeCounter++;
	}
	void create_light(const glm::vec3& position, GLuint shader) {
		Light light(position, shader);
		lights.insert(std::make_pair(lightCounter, std::make_unique<Light>(light)));
		lightCounter++;
	}
	void draw_all_lights() {
		for (auto&& light : lights) {
			draw_light(light.first, light.second);
		}
	}
	void draw_light(int lightID, const std::unique_ptr<Light>& light) {
		glm::mat4 MVP = camera.calculate_mvp(light->get_position());

		GLuint shader = light->get_shader();
		glUseProgram(shader);
		GLuint matrixID = glGetUniformLocation(shader, "MVP");

		GLCall(glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]));

		// vertex buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, light->get_vertex_buffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 24);

	}

	void draw_all_cubes() {
		for (auto&& cube : cubes) {
			draw_cube(cube.first, cube.second);			
		}
	}
	void draw_cube(int cubeID, const std::unique_ptr<Cube>& cube) {
		glm::mat4 MVP = camera.calculate_mvp(cube->get_position());

		GLuint shader = cube->get_shader();
		glUseProgram(shader);
		GLuint matrixID = glGetUniformLocation(shader, "MVP");

		GLCall(glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]));

		// vertex buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cube->get_vertex_buffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);

		// color buffer
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube->get_index_buffer());

		GLCall(glDrawElements(GL_TRIANGLES, cube->get_indices().size(), GL_UNSIGNED_INT, 0));
	}
};