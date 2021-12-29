#pragma once

#include "gui/table.h"
#include "input_manager.h"
class DebugTable : public Table
{
	void DrawTable() override {
		ImGui::Begin("Debug Table");
		ImGui::Checkbox("Draw Gizmos", &InputManager::DRAW_GIZMOS);
		ImGui::End();
	}

};
