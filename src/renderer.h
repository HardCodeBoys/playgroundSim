#pragma once
#include <glm/glm.hpp>

#include <map>
#include <set>
#include <memory>

#include "GLHelper.h"
#include "mesh/mesh.h"
#include "mesh/cube.h"
#include "mesh/plane.h"
#include "camera.h"
#include "light.h"

class Renderer {
private:
	std::map<int, std::unique_ptr<Cube>> cubes;
	std::map<int, std::unique_ptr<Light>> lights;
	std::map<int, std::unique_ptr<Plane>> planes;

	std::map<GLuint, Shader> shaders;

	int cubeCounter;
	int lightCounter;
	int planeCounter;

	Camera camera;

public:
	Renderer() : camera(glm::vec3(4, 0, 7)), cubeCounter(0), lightCounter(0), planeCounter(0) {}
	void set_camera_position(const glm::vec3& _cameraPosition) {
		camera.set_position(_cameraPosition);
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

	void create_cube(const glm::vec3& position, float size, const Shader& shader) {
		Cube cube(position, size, shader.get_id());
		if(shaders.find(shader.get_id()) == shaders.end())
			shaders.insert(std::make_pair(shader.get_id(), Shader(shader)));
		cubes.insert(std::make_pair(cubeCounter, std::make_unique<Cube>(cube)));
		cubeCounter++;
	}
	void create_light(const glm::vec3& position, const Shader& shader) {
		Light light(position, shader.get_id());
		if (shaders.find(shader.get_id()) == shaders.end())
			shaders.insert(std::make_pair(shader.get_id(), Shader(shader)));
		lights.insert(std::make_pair(lightCounter, std::make_unique<Light>(light)));
		lightCounter++;
	}
	void create_plane(const glm::vec3& position, float size, const Shader& shader) {
		Plane plane(position, size, shader.get_id());
		if (shaders.find(shader.get_id()) == shaders.end())
			shaders.insert(std::make_pair(shader.get_id(), Shader(shader)));
		planes.insert(std::make_pair(planeCounter, std::make_unique<Plane>(plane)));
		planeCounter++;
	}

	void draw_all_planes() {
		for (auto&& plane : planes) {
			draw_plane(plane.first, plane.second);
		}
	}

	void draw_all_lights() {
		for (auto&& light : lights) {
			draw_light(light.first, light.second);
		}

	}
	void draw_light(int lightID, const std::unique_ptr<Light>& light) {
		glm::mat4 MVP = camera.calculate_mvp(light->get_position());

		shaders[light->get_shader_ID()].use();
		shaders[light->get_shader_ID()].set_mat4("MVP", MVP);

		// vertex buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, light->get_vertex_buffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

	}


	void print_all_cubes_vertex_data() {
		for (auto&& cube : cubes) {
			cube.second->print_vertex_data();
		}
	}

	void draw_all_cubes() {
		for (auto&& cube : cubes) {
			draw_cube(cube.first, cube.second);			
		}
	}
	void draw_cube(int cubeID, const std::unique_ptr<Cube>& cube) {
		glm::mat4 projection = camera.get_projection_matrix();
		glm::mat4 view = camera.get_view_matrix();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), cube->get_position());	

		shaders[cube->get_shader_ID()].use();
		shaders[cube->get_shader_ID()].set_mat4("projection", projection);
		shaders[cube->get_shader_ID()].set_mat4("view", view);
		shaders[cube->get_shader_ID()].set_mat4("model", model);

		shaders[cube->get_shader_ID()].set_vec3("viewPos", camera.get_position());		

		// vertex buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cube->get_vertex_buffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);

		//normals buffer
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cube->get_vertex_buffer());
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3) * 2, (void*)(sizeof(glm::vec3)));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		// draw the normals of the cube

		auto vertexData = cube->get_vertex_data();

		glBegin(GL_LINES);
		{
			for (size_t i = 0; i < vertexData.size(); i+=2)
			{
				const auto& v = vertexData[i];
				const auto& d = vertexData[i + 1];
				glVertex3f(v.x, v.y, v.z);
				glVertex3f(v.x + d.x, v.y + d.y, v.z + d.z);
			}
		}
		glEnd();
		glUseProgram(0);
	}

	void draw_plane(int planeID, const std::unique_ptr<Plane>& plane) {

		glm::mat4 projection = camera.get_projection_matrix();
		glm::mat4 view = camera.get_view_matrix();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), plane->get_position());

		shaders[plane->get_shader_id()].use();
		shaders[plane->get_shader_id()].set_mat4("projection", projection);
		shaders[plane->get_shader_id()].set_mat4("view", view);
		shaders[plane->get_shader_id()].set_mat4("model", model);


		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, plane->get_vertex_buffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, plane->get_vertex_buffer());
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3) * 2, (void*)(sizeof(glm::vec3)));
		
		glDrawArrays(GL_TRIANGLES, 0, 12);
	}

};