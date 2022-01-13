#include "playgroundpch.h"
#include "renderer.h"

#include "utils/log.h"

void Renderer::AddShader(const Shader& shader) {
	if (shaders.find(shader.ID) == shaders.end()) {
		shaders.emplace(std::make_pair(shader.ID, shader.ID));
	}
}
void Renderer::Init() {
	// for batch rendering ? not sure if even makes sense
	glCreateVertexArrays(1, &VA);
	glBindVertexArray(VA);

	glCreateBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(VB, sizeof(glm::vec3) * 2 * MAX_VERTEX_COUNT, nullptr, GL_DYNAMIC_DRAW);

	// positions
	glEnableVertexArrayAttrib(VA, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void*)0);
	// normals
	glEnableVertexArrayAttrib(VA, 1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3) * 2, (void*)(sizeof(glm::vec3)));
}

void Renderer::DrawModel(const std::shared_ptr<Model>& model, bool withNormals) {
	glm::mat4 projection = camera.GetProjectionMatrix();
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 modelMat = glm::mat4(1.0f);

	GLuint shaderID = model->shaderID;

	shaders[shaderID].Use();
	shaders[shaderID].SetMat4("projection", projection);
	shaders[shaderID].SetMat4("view", view);
	shaders[shaderID].SetMat4("model", modelMat);

	shaders[shaderID].SetVec3("viewPos", camera.position);

	unsigned int stride = sizeof(glm::vec3) * 2;

	glBindBuffer(GL_ARRAY_BUFFER, model->GetVertexBuffer());
	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, stride, (void*)(sizeof(glm::vec3)));

	// for simple cubes and planes, stupid, TODO: remake
	if (model->indices.size() == 0) {
		glDrawArrays(GL_TRIANGLES, 0, model->vertexData.size());
	}
	// for other entities
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetIndexBuffer());

		glDrawElements(GL_TRIANGLES,
			model->indices.size(),
			GL_UNSIGNED_INT,
			0);
	}

	if (withNormals) {
		// draw the normals of the models
		auto& vertexData = model->vertexData;

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
	}
	glUseProgram(0);
}

void Renderer::DrawLine(const glm::vec3& a, const glm::vec3& b, GLuint shaderID) {
	glm::mat4 projection = camera.GetProjectionMatrix();
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 modelMat = glm::mat4(1.0f);

	shaders[shaderID].Use();
	shaders[shaderID].SetMat4("projection", projection);
	shaders[shaderID].SetMat4("view", view);
	shaders[shaderID].SetMat4("model", modelMat);

	shaders[shaderID].SetVec3("viewPos", camera.position);

	glBegin(GL_LINES);
	{
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
	}
	glEnd();

}
// TODO: should not get a light, just its model, maybe shouldn't draw the light at all, just its Gizmo
void Renderer::DrawLight(const std::unique_ptr<Light>& light) {
	glm::mat4 MVP = camera.CalculateMVP(light->position);
	GLuint shaderID = light->shaderID;

	shaders[shaderID].Use();

	shaders[shaderID].SetMat4("MVP", MVP);

	// vertex buffer
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, light->GetVertexBuffer());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}