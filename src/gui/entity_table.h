#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "table.h"
#include "scene/entity.h"

class EntityTable : public Table
{
private:
	std::shared_ptr<Entity> entity;
public:
	EntityTable(const std::shared_ptr<Entity>& _entity) {
		entity = _entity;
	}
	void DrawTable() override {
		ImGui::Begin(typeid(entity).name());
		std::string position = "entity pos: " + glm::to_string(entity->position);
		ImGui::Text(position.c_str());
		position = "model pos: " + glm::to_string(entity->model->position);
		ImGui::Text(position.c_str());
		position = "collider pos: " + glm::to_string(entity->collider->position);
		ImGui::Text(position.c_str());
		position = "collider model pos: " + glm::to_string(entity->collider->model->position);
		ImGui::Text(position.c_str());
		ImGui::End();
	}
};