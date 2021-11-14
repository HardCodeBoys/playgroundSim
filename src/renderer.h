#pragma once
#include <glm/glm.hpp>

#include <map>
#include <set>
#include <memory>

#include "GLHelper.h"
#include "cube.h"
#include "plane.h"
#include "camera.h"
#include "light.h"

class Renderer {
private:
	std::map<GLuint, Shader> shaders;

	Camera camera;
public:
	Renderer() : camera(glm::vec3(4, 0, 7)) {}
	void set_camera_position(const glm::vec3& _cameraPosition) {
		camera.set_position(_cameraPosition);
	}
	Camera& get_camera() {
		return camera;
	}
	glm::vec3 get_camera_position() {
		return camera.get_position();
	}
	void add_shader(const Shader& shader) {
		if(shaders.find(shader.get_id()) == shaders.end())
			shaders.insert(std::make_pair(shader.get_id(), Shader(shader)));
	}

	void draw_object(const std::unique_ptr<Object>& object, bool withNormals = false) {

		glm::mat4 projection = camera.get_projection_matrix();
		glm::mat4 view = camera.get_view_matrix();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), object->get_position());

		shaders[object->get_shaderID()].use();
		shaders[object->get_shaderID()].set_mat4("projection", projection);
		shaders[object->get_shaderID()].set_mat4("view", view);
		shaders[object->get_shaderID()].set_mat4("model", model);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, object->get_vertex_buffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, object->get_vertex_buffer());
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3) * 2, (void*)(sizeof(glm::vec3)));

		glDrawArrays(GL_TRIANGLES, 0, object->get_num_of_vertices());

		if (withNormals) {
			// draw the normals of the object

			auto vertexData = object->get_vertex_data();

			glBegin(GL_LINES);
			{
				for (size_t i = 0; i < vertexData.size(); i += 2)
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
	}
	void draw_light(const std::unique_ptr<Light>& light) {
		glm::mat4 MVP = camera.calculate_mvp(light->get_position());

		shaders[light->get_shader_ID()].use();
		shaders[light->get_shader_ID()].set_mat4("MVP", MVP);

		// vertex buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, light->get_vertex_buffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

	}
};