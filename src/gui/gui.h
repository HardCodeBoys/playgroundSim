#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "entity_table.h"

class GUI
{
private:
	inline static std::vector<std::shared_ptr<Table>> tables;

public:
	static void Init(GLFWwindow* window) {
		// ------- IMGUI INITIALIZATION -------
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 440");
		// --------------
	}
	static void AddTable(const std::shared_ptr<Table>& table) {
		tables.push_back(table);
	}
	static void DrawTables() {
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		for (size_t i = 0; i < tables.size(); i++)
		{
			tables[i]->DrawTable();
		}
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};